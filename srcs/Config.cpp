/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:51:24 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/27 12:52:00 by kbaridon         ###   ########.fr       */
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
	n = 1;
	while (n > 0)
	{
		n = read(fd, buf, 4096);
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


Config::~Config() {}
Config::Config() {}

Config::Config(const std::string &path)
{
	std::string raw = readFile(path);

	//parsing et init a faire
	if (this->servers.empty())
		throw (Config::InvalidFileException());
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

char const *Config::InvalidFileException::what(void) const throw() {
	return ("Invalid configuration file.");
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
