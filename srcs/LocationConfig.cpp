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

void	LocationConfig::setPath(const std::string &p) {
	this->path = p;
}

void	LocationConfig::setRoot(const std::string &root) {
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
	stream << "    Path: " << src.getPath() << std::endl;
	stream << "    Root: " << src.getRoot() << std::endl;
	stream << "    Index: " << src.getIndex() << std::endl;
	stream << "    Autoindex: " << (src.getAutoIndex() ? "on" : "off") << std::endl;

	const std::vector<std::string>& methods = src.getAllowedMethods();
	if (!methods.empty()) {
		stream << "    Allowed Methods: ";
		for (size_t i = 0; i < methods.size(); ++i) {
			if (i > 0) stream << ", ";
				stream << methods[i];
		}
		stream << std::endl;
	}
	if (!src.getCgiExtension().empty()) {
		stream << "    CGI: " << src.getCgiExtension() << " -> " << src.getCgiPath() << std::endl;
	}
	if (!src.getRedirect().empty()) {
		stream << "    Redirect: " << src.getRedirect() << std::endl;
	}
	if (!src.getUploadPath().empty()) {
		stream << "    Upload Path: " << src.getUploadPath() << std::endl;
	}
	return (stream);
}
