/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:38:37 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/09 01:07:13 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

void	Request::process()
{
	this->_method = get_first_word(this->_serialized_request);
	this->_uri = get_first_word(this->_serialized_request);
	this->_http_version = get_http_version(this->_serialized_request);
	if (this->_method.empty() || this->_uri.empty() || this->_http_version == 0)
	{
		this->_error = true;
		return ;
	}
	else
		this->_error = false;
	this->_ressource_path = find_ressource_path(this->_uri);
	this->_query_string = find_query_string(this->_uri);
	this->_serialized_request = this->_serialized_request.substr(this->_serialized_request.find_first_not_of(" \t\n\r"));
	std::string line = get_next_line(this->_serialized_request);
	while (!line.empty() && line != "\r\n")
	{
		size_t mid = line.find(':');
		if (mid == std::string::npos) {
			line = get_next_line(this->_serialized_request);
			continue;
		}
		std::string key = trim(line.substr(0, mid));
		std::string value = trim(line.substr(mid + 1));
		_headers.push_back(std::make_pair(key, value));
		line = get_next_line(this->_serialized_request);
	}
	this->_body.swap(this->_serialized_request);
}

void	Request::setUri(std::string uri)
{
	this->_uri = uri;	
}

const std::string&	Request::getSerializedRequest() const {
	return (this->_serialized_request);
}

const std::string&	Request::getMethod() const {
	return (this->_method);
}

const std::string&	Request::getUri() const {
	return (this->_uri);
}

const std::string&	Request::getRessourcePath() const
{
	return (this->_ressource_path);
}

const std::string&	Request::getQueryString() const
{
	return (this->_query_string);
}

int	Request::getSysCallVerif() const
{
	return sys_call_verif;
}

void	Request::setSysCallVerif(int i)
{
	sys_call_verif = i;
}


Request::Request(std::string serialized_request) : Message(), _serialized_request(serialized_request)
{
	this->process();		
}

Request::Request(const Request& copy) : Message(copy)
{
	*this = copy;
}

Request::~Request() {}

Request&	Request::operator=(const Request& src)
{
	if (this != &src)
	{
		Message::operator=(src);
		this->_uri = src._uri;
		this->_method = src._method;
		this->_uri = src._uri;
		this->_serialized_request = src._serialized_request;
	}
	return (*this);
}
