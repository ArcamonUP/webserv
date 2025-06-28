/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:51:24 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/28 12:35:45 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Webserv.hpp"
#include <fcntl.h>
#include <unistd.h>

static std::string readFile(const std::string& path)
{
	int			fd;
	char		buf[4096];
	ssize_t		n;
	std::string	out;

	fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		throw Config::InvalidFileException();
	while ((n = read(fd, buf, 4096)) > 0)
	{
		out.append(buf, n);
	}
	close(fd);
	if (n < 0)
		throw Config::InvalidFileException();
	return (out);
}

static int	ft_atoi(const std::string value)
{
	std::size_t	i = 0;
	int			sign = 1;
	long		result = 0;

	while (i < value.length() && (value[i] == ' ' || value[i] == '\t' || value[i] == '\n'
		|| value[i] == '\v' || value[i] == '\f' || value[i] == '\r'))
		i++;
	if (i < value.length() && (value[i] == '-' || value[i] == '+'))
	{
		if (value[i] == '-')
			sign = -1;
		i++;
	}
	while (i < value.length() && value[i] >= '0' && value[i] <= '9')
	{
		result = result * 10 + (value[i] - '0');
		i++;
	}

	return (static_cast<int>(sign * result));
}

static std::string	getToken(std::string file, size_t *pos)
{
	std::string	result;

	if (*pos >= file.length())
		return ("");
	if (file[*pos] == ';') {(*pos)++; return (";");}
	else if (file[*pos] == '{') {(*pos)++; return ("{");}
	else if (file[*pos] == '}') {(*pos)++; return ("}");}
	while (*pos < file.length() && !std::isspace(file[*pos]) && \
		file[*pos] != '{' && file[*pos] != '}' && file[*pos] != ';')
	{
		result += file[*pos];
		(*pos)++;
	}
	return (result);
}

static void	skipSpacesComments(std::string raw, size_t *pos)
{
	while (*pos < raw.length() && (std::isspace(raw[*pos]) || raw[*pos] == '#'))
	{
		if (raw[*pos] == '#')
		{
			while (*pos < raw.length() && raw[*pos] != '\n')
			{
				(*pos)++;
			}
		}
		else
			(*pos)++;
	}
}

static void	skipLine(std::string raw, size_t *pos)
{
	while (*pos < raw.length() && (std::isspace(raw[*pos])))
		(*pos)++;
	if (*pos < raw.length() && raw[*pos] == ';')
		(*pos)++;
	return ;
}

static size_t	ft_convert(std::string str)
{
	str = trim(str);
	if (str.empty())
		throw std::invalid_argument("empty max_size_body");

	size_t mul = 1;
	char suf = str[str.size() - 1];
	if (!std::isdigit(static_cast<unsigned char>(suf)))
	{
		switch (suf)
		{
			case 'K': case 'k': mul = 1024ULL; break;
			case 'M': case 'm': mul = 1024ULL * 1024ULL; break;
			case 'G': case 'g': mul = 1024ULL * 1024ULL * 1024ULL; break;
			default: throw std::invalid_argument("invalid max_size_body suffix: must be K, M, or G");
		}
		str.erase(str.size() - 1);
	}
		if (str.empty())
		throw std::invalid_argument("invalid max_size_body: missing numeric value");
	int val = ft_atoi(str);
	if (val < 0)
		throw std::invalid_argument("invalid max_size_body: value must be positive");
	
	size_t result = static_cast<size_t>(val) * mul;
		if (result / mul != static_cast<size_t>(val))
		throw std::invalid_argument("invalid max_size_body: value too large");
	return result;
}

static bool	is_all_digit(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
	}
	return (true);
}

static ServerConfig getPages(std::string raw, size_t *pos, ServerConfig result)
{
	std::vector<int>			code_list;
	std::map<int, std::string>	error_pages = result.getErrorPages();

	skipSpacesComments(raw, pos);
	if (*pos >= raw.length())
		throw(Config::InvalidFileException());
	while (*pos < raw.length())
	{
		std::string token = getToken(raw, pos);
		skipSpacesComments(raw, pos);
		if (*pos >= raw.length())
			throw(Config::InvalidFileException());
		if (!is_all_digit(token))
		{
			if (token.empty())
				throw std::invalid_argument("Unexpected end of line while parsing error_page directive.");
			std::string url = token;
			for (size_t i = 0; i < code_list.size(); ++i)
				error_pages[code_list[i]] = url;
			skipLine(raw, pos);
			result.setErrorPages(error_pages);
			return (result);
		}
		else
			code_list.push_back(ft_atoi(token));
	}
	throw std::invalid_argument("Missing URL for error_page directive.");
}
static LocationConfig	getCgi(std::string raw, size_t *pos, LocationConfig loc)
{
	std::string	extension;
	std::string	path;

	skipSpacesComments(raw, pos);
	if (*pos >= raw.length())
		throw(Config::InvalidFileException());
	
	extension = getToken(raw, pos);
	if (extension.empty() || extension == ";")
		throw std::invalid_argument("Missing CGI extension in cgi directive.");
	
	skipSpacesComments(raw, pos);
	if (*pos >= raw.length())
		throw(Config::InvalidFileException());
	
	path = getToken(raw, pos);
	if (path.empty() || path == ";")
		throw std::invalid_argument("Missing CGI path in cgi directive.");
	
	loc.setCgiExtension(extension);
	loc.setCgiPath(path);
	skipLine(raw, pos);
	return (loc);
}

static LocationConfig	getMethods(std::string raw, size_t *pos, LocationConfig loc)
{
	std::vector<std::string> methods;

	skipSpacesComments(raw, pos);
	if (*pos >= raw.length())
		throw(Config::InvalidFileException());
	while (*pos < raw.length())
	{
		std::string token = getToken(raw, pos);
		skipSpacesComments(raw, pos);
		if (*pos >= raw.length())
			throw(Config::InvalidFileException());
		if (token.empty())
			throw std::invalid_argument("Unexpected end of line while parsing methods directive.");
		if (token == ";")
			break ;
		methods.push_back(token);
	}
	if (methods.empty())
		throw std::invalid_argument("At least one HTTP method must be specified");
	
	loc.setAllowedMethods(methods);
	return (loc);
}

static LocationConfig	getLocationValue(std::string raw, size_t *pos, LocationConfig loc, std::string key)
{
	std::string	value;
	std::string	possibles_str[] = {"root", "index", "redirect", "upload_path"};
	void		(LocationConfig::*functions[])(const std::string &value) = {&LocationConfig::setRoot, &LocationConfig::setIndex, \
		&LocationConfig::setRedirect, &LocationConfig::setUploadPath};

	skipSpacesComments(raw, pos);
	if (*pos >= raw.length())
		throw(Config::InvalidFileException());
	for (size_t i = 0; i < 4; i++) {
		if (possibles_str[i] == key) {
			(loc.*functions[i])(getToken(raw, pos));
			return (skipLine(raw, pos), loc);
		}
	}
	if (key == "autoindex") {
		if (getToken(raw, pos) == "on")
			loc.setAutoIndex(true);
		else
			loc.setAutoIndex(false);
		return (skipLine(raw, pos), loc);
	}
	if (key == "cgi")
		return (getCgi(raw, pos, loc));
	if (key == "methods")
		return(getMethods(raw, pos, loc));
	std::cerr << "webserv: unknow directive '" << key << "'" << " inside a location block." << std::endl; 
	throw(Config::InvalidFileException());
}

ServerConfig	getLocationBlock(std::string raw, size_t *pos, ServerConfig result)
{
	skipSpacesComments(raw, pos);
	if (*pos >= raw.length())
		throw(Config::InvalidFileException());
	std::string path = getToken(raw, pos);
	skipSpacesComments(raw, pos);

	if (*pos >= raw.length() || raw[*pos] != '{')
		throw (Config::InvalidFileException());
	(*pos)++;
	LocationConfig location;
	location.setPath(path);
	while (*pos < raw.length())
	{
		skipSpacesComments(raw, pos);
		if (*pos >= raw.length())
			throw(Config::InvalidFileException());
		if (raw[*pos] == '}') {(*pos)++; break;}
		std::string key = getToken(raw, pos);
		location = getLocationValue(raw, pos, location, key);
	}
	result.addLocation(location);
	return (result);
}

static ServerConfig	getServerValue(std::string raw, size_t *pos, ServerConfig result, const std::string& key)
{
	std::string	value;
	std::string possibles_str[] = {"server_name", "root", "index"};
	void		(ServerConfig::*functions[])(const std::string &value) = {&ServerConfig::setServerName, &ServerConfig::setRoot, &ServerConfig::setIndex};
	
	skipSpacesComments(raw, pos);
	if (*pos >= raw.length())
		throw(Config::InvalidFileException());	
	for (size_t i = 0; i < 3; i++) {
		if (possibles_str[i] == key) {
			(result.*functions[i])(getToken(raw, pos));
			return (skipLine(raw, pos), result);
		}
	}
	if (key == "listen") {
		result.setPort(ft_atoi(getToken(raw, pos)));
		return (skipLine(raw, pos), result);
	}
	if (key == "max_size_body") {
		result.setMaxSizeBody(ft_convert(getToken(raw, pos)));
		return (skipLine(raw, pos), result);
	}
	if (key == "error_page")
		return (getPages(raw, pos, result));
	if (key == "location")
		return (getLocationBlock(raw, pos, result));
	std::cerr << "webserv: unknow directive '" << key << "'" << std::endl; 
	throw(Config::InvalidFileException());
}

static ServerConfig	getServerConfig(std::string raw, size_t *pos)
{
	ServerConfig	result;
	std::string		key;

	while (*pos < raw.length() && raw[*pos] != '{')
		(*pos)++;
	if (*pos >= raw.length())
		throw(Config::InvalidFileException());
	(*pos)++;
	while (*pos < raw.length())
	{
		skipSpacesComments(raw, pos);
		if (*pos >= raw.length())
			throw(Config::InvalidFileException());
		if (raw[*pos] == '}')
			return ((*pos)++, result);
		key = getToken(raw, pos);
		result = getServerValue(raw, pos, result, key);
	}
	return (result);
}

Config::~Config() {}
Config::Config() {}

Config::Config(const std::string &path)
{
	std::string raw = readFile(path);
	size_t		pos = 0;

	while (pos < raw.length())
	{
		skipSpacesComments(raw, &pos);
		if (pos >= raw.length())
			break ;
		std::string token = getToken(raw, &pos);
		if (token == "server")
		{
			ServerConfig server = getServerConfig(raw, &pos);
			this->servers.push_back(server);
		}
		else if (!token.empty())
			throw (Config::InvalidFileException());
	}
	if (this->servers.empty())
		throw (Config::InvalidFileException());
	validateConfiguration();
}

Config::Config(const Config &copy) {
	*this = copy;
}

Config &Config::operator=(const Config &src) {
	if (this != &src)
		this->servers = src.servers;
	return (*this);
}

std::vector<ServerConfig>	Config::getServer(void) const {
	return (this->servers);
}

void Config::validateConfiguration() {
	for (size_t i = 0; i < servers.size(); ++i) {
		for (size_t j = i + 1; j < servers.size(); ++j) {
			if (servers[i].getPort() == servers[j].getPort() && 
				servers[i].getServerName() == servers[j].getServerName()) {
				throw std::invalid_argument("Duplicate server configuration: same port and server_name");
			}
		}
		if (servers[i].getPort() <= 0) {
			throw std::invalid_argument("Server must have a valid port defined");
		}
		const std::vector<LocationConfig>& locations = servers[i].getLocations();
		for (size_t k = 0; k < locations.size(); ++k) {
			for (size_t l = k + 1; l < locations.size(); ++l) {
				if (locations[k].getPath() == locations[l].getPath()) {
					throw std::invalid_argument("Duplicate location path: " + locations[k].getPath());
				}
			}
			if (locations[k].getAllowedMethods().empty()) {
				throw std::invalid_argument("Location " + locations[k].getPath() + " must have at least one allowed method");
			}
		}
	}
}

std::ostream &operator<<(std::ostream &stream, const Config &src) {
	stream << "=== WEBSERV CONFIGURATION ===" << std::endl;
	stream << "Number of servers: " << src.getServer().size() << std::endl;
	
	for (size_t i = 0; i < src.getServer().size(); ++i) {
		stream << std::endl << "--- SERVER " << i + 1 << " ---" << std::endl;
		stream << src.getServer()[i];
	}

	stream << std::endl << "=== END CONFIGURATION ===" << std::endl;
	return (stream);
}

char const *Config::InvalidFileException::what(void) const throw() {
	return ("Invalid configuration file.");
}

