/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:51:24 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/26 16:39:43 by kbaridon         ###   ########.fr       */
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

Config::~Config() {}

Config::Config(const std::string &path)
{
	std::string	raw = readFile(path);
	
}

Config::Config(const Config &copy) {
	*this = copy;
}

Config &Config::operator=(const Config &src) {
	if (this != &src)
		this->server = src.server;
	return (*this);
}

ServerConfig	Config::getServer(void) const {
	return (this->server);
}

char const *Config::InvalidFileException::what(void) const throw() {
	return ("Invalid configuration file.");
}

// std::ostream &operator<<(std::ostream &stream, const Config &src) {
// 	stream << src.getServer();
// 	return (stream);
// }
