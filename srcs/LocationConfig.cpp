/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:08:31 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/26 16:28:29 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"

LocationConfig::LocationConfig() : path(""), root(""), index("index.html"), redirect(""), autoindex(false), cgi_extension(""), cgi_path(""), upload_path(""), allowed_methods()
{
	allowed_methods.push_back("GET");
}

void	LocationConfig::setPath(const std::string &p) {
	if (p.empty())
		throw std::invalid_argument("Location path cannot be empty");
	if (p[0] != '/')
		throw std::invalid_argument("Location path must start with '/'");
	this->path = p;
}

void	LocationConfig::setRoot(const std::string &root) {
	if (root.empty())
		throw std::invalid_argument("Root path cannot be empty");
	this->root = root;
}

void	LocationConfig::setIndex(const std::string &i) {
	this->index = i;
}

void	LocationConfig::setRedirect(const std::string &redir) {
	this->redirect = redir;
}

void	LocationConfig::setAutoIndex(const bool value) {
	this->autoindex = value;
}

void	LocationConfig::setCgiExtension(const std::string &cgi_e) {
	this->cgi_extension = cgi_e;
}

void	LocationConfig::setCgiPath(const std::string &cgi_p) {
	this->cgi_path = cgi_p;
}

void	LocationConfig::setUploadPath(const std::string &up) {
	this->upload_path = up;
}

void	LocationConfig::setAllowedMethods(const std::vector<std::string> &m) {
	for (size_t i = 0; i < m.size(); ++i) {
		const std::string &method = m[i];
		if (method != "GET" && method != "POST" && method != "DELETE" && 
			method != "PUT" && method != "HEAD" && method != "OPTIONS") {
			throw std::invalid_argument("Invalid HTTP method: " + method);
		}
	}
	this->allowed_methods = m;
}

const std::string	&LocationConfig::getPath() const {
	return (this->path);
}

const std::string	&LocationConfig::getRoot() const {
	return (this->root);
}

const std::string	&LocationConfig::getIndex() const {
	return (this->index);
}

const std::string	&LocationConfig::getRedirect() const {
	return (this->redirect);
}

bool	LocationConfig::getAutoIndex() const {
	return (this->autoindex);
}

const std::string	&LocationConfig::getCgiExtension() const {
	return (this->cgi_extension);
}

const std::string	&LocationConfig::getCgiPath() const {
	return (this->cgi_path);
}

const std::string	&LocationConfig::getUploadPath() const {
	return (this->upload_path);
}

const std::vector<std::string>	&LocationConfig::getAllowedMethods() const {
	return (this->allowed_methods);
}

std::ostream &operator<<(std::ostream &stream, const LocationConfig &src) {
    if (!src.getPath().empty()) {
        stream << "    Path: " << src.getPath() << std::endl;
    } else {
        stream << "    Path: [NOT SET]" << std::endl;
    }
    if (!src.getRoot().empty()) {
        stream << "    Root: " << src.getRoot() << std::endl;
    } else {
        stream << "    Root: [NOT SET]" << std::endl;
    }
    if (!src.getIndex().empty()) {
        stream << "    Index: " << src.getIndex() << std::endl;
    } else {
        stream << "    Index: [NOT SET]" << std::endl;
    }
    stream << "    Autoindex: " << (src.getAutoIndex() ? "on" : "off") << std::endl;
    const std::vector<std::string>& methods = src.getAllowedMethods();
    if (!methods.empty()) {
        stream << "    Allowed Methods: ";
        for (size_t i = 0; i < methods.size(); ++i) {
            if (i > 0) stream << ", ";
                stream << methods[i];
        }
        stream << std::endl;
    } else {
        stream << "    Allowed Methods: [NOT SET]" << std::endl;
    }
    if (!src.getCgiExtension().empty() && !src.getCgiPath().empty()) {
        stream << "    CGI: " << src.getCgiExtension() << " -> " << src.getCgiPath() << std::endl;
    } else if (!src.getCgiExtension().empty() || !src.getCgiPath().empty()) {
        stream << "    CGI: [PARTIALLY SET - Extension: " << src.getCgiExtension() 
               << ", Path: " << src.getCgiPath() << "]" << std::endl;
    } else {
        stream << "    CGI: [NOT SET]" << std::endl;
    }
    if (!src.getRedirect().empty()) {
        stream << "    Redirect: " << src.getRedirect() << std::endl;
    } else {
        stream << "    Redirect: [NOT SET]" << std::endl;
    }
    if (!src.getUploadPath().empty()) {
        stream << "    Upload Path: " << src.getUploadPath() << std::endl;
    } else {
        stream << "    Upload Path: [NOT SET]" << std::endl;
    }
    return (stream);
}
