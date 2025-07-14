/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:14:49 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/14 14:43:18 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <cstring>

ServerConfig::ServerConfig() : host("127.0.0.1"), port(80), max_size_body(1048576), root("./"), index("index.html"), error_pages(), locations(), sockfd(-1)
{
    std::memset(&sockaddr, 0, sizeof(sockaddr));
}

void ServerConfig::setHost(const std::string &ip)
{
    if (!check_ip(ip))
        throw std::invalid_argument("Invalid IP address.");
    this->host = ip;
}

void ServerConfig::setPort(int p)
{
    if (p <= 0 || p > 65535)
        throw std::invalid_argument("Invalid port number: must be between 1 and 65535");
    this->port = p;
}

void ServerConfig::setMaxSizeBody(size_t max)
{
    this->max_size_body = max;
}

void ServerConfig::setRoot(const std::string &r)
{
    if (r.empty())
        throw std::invalid_argument("Root path cannot be empty");
    this->root = r;
}

void ServerConfig::setIndex(const std::string &i)
{
    this->index = i;
}

void ServerConfig::setErrorPages(const std::map<int, std::string> &e)
{
    this->error_pages = e;
}

void ServerConfig::addErrorPage(int code, std::string page)
{
    this->error_pages.insert(std::make_pair(code, page));
}

void ServerConfig::setLocations(const std::vector<LocationConfig> &l)
{
    this->locations = l;
}

void ServerConfig::addLocation(const LocationConfig loc)
{
    this->locations.push_back(loc);
}

void ServerConfig::setSockfd(int fd)
{
    this->sockfd = fd;
}

void ServerConfig::setSockaddr(const sockaddr_in& addr)
{
    this->sockaddr = addr;
}

const std::string &ServerConfig::getHost() const
{
    return (this->host);
}

int ServerConfig::getPort() const
{
    return (this->port);
}

size_t ServerConfig::getMaxSizeBody() const
{
    return (this->max_size_body);
}

const std::string &ServerConfig::getRoot() const
{
    return (this->root);
}

const std::string &ServerConfig::getIndex() const
{
    return (this->index);
}

const std::map<int, std::string> &ServerConfig::getErrorPages() const
{
    return (this->error_pages);
}

const std::vector<LocationConfig> ServerConfig::getLocations() const
{
    return (this->locations);
}

int ServerConfig::getSockfd() const
{
    return this->sockfd;
}

const sockaddr_in& ServerConfig::getSockaddr() const
{
    return this->sockaddr;
}

std::ostream &operator<<(std::ostream &stream, const ServerConfig &src)
{
    if (src.getPort() != 0) {
        stream << "Port: " << src.getPort() << std::endl;
    } else {
        stream << "Port: [NOT SET]" << std::endl;
    }
    
    if (!src.getHost().empty()) {
        stream << "Host : " << src.getHost() << std::endl;
    } else {
        stream << "Host : [NOT SET]" << std::endl;
    }
    
    if (!src.getRoot().empty()) {
        stream << "Root: " << src.getRoot() << std::endl;
    } else {
        stream << "Root: [NOT SET]" << std::endl;
    }
    
    if (!src.getIndex().empty()) {
        stream << "Index: " << src.getIndex() << std::endl;
    } else {
        stream << "Index: [NOT SET]" << std::endl;
    }
    
    if (src.getMaxSizeBody() != 0) {
        stream << "Max Body Size: " << src.getMaxSizeBody() << std::endl;
    } else {
        stream << "Max Body Size: [NOT SET]" << std::endl;
    }
    
    const std::map<int, std::string> &errorPages = src.getErrorPages();
    if (!errorPages.empty())
    {
        stream << "Error Pages:" << std::endl;
        for (std::map<int, std::string>::const_iterator it = errorPages.begin();
             it != errorPages.end(); ++it)
        {
            stream << "  " << it->first << " -> " << it->second << std::endl;
        }
    } else {
        stream << "Error Pages: [NOT SET]" << std::endl;
    }
    
    const std::vector<LocationConfig> &locations = src.getLocations();
    if (!locations.empty())
    {
        stream << "Locations (" << locations.size() << "):" << std::endl;
        for (size_t i = 0; i < locations.size(); ++i)
        {
            stream << "  Location " << i + 1 << ":" << std::endl;
            stream << locations[i];
        }
    } else {
        stream << "Locations: [NOT SET]" << std::endl;
    }
    return (stream);
}
