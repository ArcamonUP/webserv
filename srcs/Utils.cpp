/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:17:45 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/09 17:17:07 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

void	initMethodMap()
{
	method_map["HEAD"] = &HandleHEAD;
	method_map["GET"] = &HandleGET;
	method_map["POST"] = &HandlePOST;
	method_map["DELETE"] = &HandleDELETE;
}

int	make_not_blocking_socket(int fd)
{
	int	flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		return (-1);
	if ((flags & O_NONBLOCK) != 0)
		return (0);
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		return (-1);
	return (0);
}

bool	endsWith(const std::string &str, const std::string &suffix)
{
	if (suffix.size() > str.size())
		return (false);
	return (str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0);
}

std::string trim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

int	ft_atoi(const std::string value)
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

bool	is_all_digit(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
	}
	return (true);
}

std::string get_file_content(const std::string& file_path)
{
	std::ifstream file(file_path.c_str());
	if (!file.is_open() || file.fail())
	{
		if (errno == ENOENT)
			throw Response::ResourceNotFoundException(); 
		else
			throw Response::InternalServerErrorException();	
	}
	std::stringstream	buffer;
	std::string file_content;

	buffer << file.rdbuf();
	file.close();
	file_content = buffer.str();
	return (file_content);
}

int find_matching_location_index(ServerConfig& conf, const std::string& uri)
{
	const std::vector<LocationConfig>& locations = conf.getLocations();
	int best_match_index = -1;
	size_t best_match_length = 0;

	for (size_t i = 0; i < locations.size(); i++) {
		const std::string& location_path = locations[i].getPath();
		if (uri.find(location_path) == 0) {
			if (location_path.length() > best_match_length) {
				best_match_index = (int)i;
				best_match_length = location_path.length();
			}
		}
	}
	
	return best_match_index;
}

bool has_custom_error_page(ServerConfig& conf, int error_code)
{
	const std::map<int, std::string>& error_pages = conf.getErrorPages();
	return error_pages.find(error_code) != error_pages.end();
}

std::string get_default_error_page(int error_code)
{
	std::ostringstream html;
	html << "<!DOCTYPE html><html><head><title>Error " << error_code << "</title>";
	html << "<style>body{font-family:'Segoe UI',sans-serif;margin:0;background:linear-gradient(135deg,#667eea,#764ba2);min-height:100vh;display:flex;align-items:center;justify-content:center}";
	html << ".container{background:white;padding:40px;border-radius:15px;box-shadow:0 20px 40px rgba(0,0,0,0.1);text-align:center;max-width:500px}";
	html << "h1{color:#e74c3c;margin-bottom:20px}p{color:#666;margin-bottom:30px}";
	html << ".btn{background:linear-gradient(45deg,#667eea,#764ba2);color:white;padding:12px 24px;border-radius:25px;text-decoration:none;display:inline-block}</style></head>";
	html << "<body><div class=\"container\"><h1>Error " << error_code << "</h1>";
	
	switch (error_code) {
		case 400: html << "<p>Invalid request.</p>"; break;
		case 403: html << "<p>Resource access is forbidden.</p>"; break;
		case 404: html << "<p>Resource not found.</p>"; break;
		case 405: html << "<p>Unauthorized method</p>"; break;
		case 500: html << "<p>Server encountered an intern error.</p>"; break;
		case 502: html << "<p>Bad Gateway.</p>"; break;
		case 503: html << "<p>Service is unavailable.</p>"; break;
		case 504: html << "<p>Timeout gateway.</p>"; break;
		default: html << "<p>An unknown error occured.</p>"; break;
	}
	return html.str();
}

std::string get_custom_error_page(ServerConfig& conf, int error_code)
{
	const std::map<int, std::string>& error_pages = conf.getErrorPages();
	std::map<int, std::string>::const_iterator it = error_pages.find(error_code);
	
	if (it != error_pages.end()) {
		std::string error_page_path = conf.getRoot() + it->second;
		try {
			return get_file_content(error_page_path);
		} catch (const Response::ResourceNotFoundException&) {
			std::cerr << "Error: Custom error page not found: " << error_page_path << std::endl;
		} catch (const Response::InternalServerErrorException&) {
			std::cerr << "Error: Cannot read custom error page: " << error_page_path << std::endl;
		} catch (const std::exception& e) {
			std::cerr << "Error reading custom error page: " << e.what() << std::endl;
		}
	}
	return get_default_error_page(error_code);
}

void free_env(char **envp)
{
    if (!envp) return;

    for (size_t i = 0; envp[i]; i++)
        delete[] envp[i];
    delete[] envp;
}