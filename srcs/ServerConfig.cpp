/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:14:49 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/26 16:36:26 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

void	ServerConfig::setPort(int p) {
	this->port = p;
}

void	ServerConfig::setServerName(const std::string &n) {
	this->server_name = n;
}

void    ServerConfig::setMaxSizeBody(size_t max) {
    this->max_size_body = max;
}

void	ServerConfig::setRoot(const std::string &r) {
	this->root = r;
}

void	ServerConfig::setIndex(const std::string &i) {
	this->index = i;
}

void	ServerConfig::setErrorPages(const std::map<int, std::string> &e) {
	this->error_pages = e;
}

void	ServerConfig::setLocations(const std::vector<LocationConfig> &l) {
	this->locations = l;
}

int ServerConfig::getPort() const {
    return (this->port);
}

const std::string	&ServerConfig::getServerName() const {
    return (this->server_name);
}

size_t	ServerConfig::getMaxSizeBody() const {
    return (this->max_size_body);
}

const std::string	&ServerConfig::getRoot() const {
    return (this->root);
}

const std::string	&ServerConfig::getIndex() const {
    return (this->index);
}

const std::map<int, std::string>	&ServerConfig::getErrorPages() const {
    return (this->error_pages);
}

const std::vector<LocationConfig>	ServerConfig::getLocations() const {
    return (this->locations);
}
