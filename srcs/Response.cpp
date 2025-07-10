/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:02:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/10 14:50:54 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

std::map<int, Response::ResponseFunction> Response::_builders;
std::map<std::string, std::string> Response::_content_types;

void	Response::process()
{
	try
	{
		(this->*_builders.at(this->_status_code))();
	}
	catch(const std::out_of_range&)
	{
		(this->*_builders[500])();
	}
}

void	Response::setStatusCode(const int status_code)
{
	this->_status_code = status_code;
}

void	Response::setStatusName(const std::string status_name)
{
	this->_status_name = status_name;
}

void	Response::setRessourcePath(const std::string requested_ressource_path)
{
	this->_ressource_path = requested_ressource_path;
}

void	Response::setContentLength(const std::string length)
{
	_headers.push_back(std::make_pair("content-length", length));
}

void	Response::setContentType(const std::string type)
{
	_headers.push_back(std::make_pair("content-type", type));
}

void	Response::setDate()
{
	_headers.push_back(std::make_pair("date", Response::getDate()));
}

void	Response::setLocation(const std::string location)
{
	_headers.push_back(std::make_pair("location", location));
}

int	Response::getStatusCode() const
{
	return (this->_status_code);
}

std::string	Response::getStatusName() const
{
	return (this->_status_name);
}

std::string Response::getRessourcePath() const
{
	return (this->_ressource_path);
}

std::string Response::getExtension(const std::string& Uri) const
{
	std::size_t dot_pos = Uri.find_last_of('.');
	
	if (dot_pos == std::string::npos)
		return ("");
	else
	{
		std::string extension = Uri.substr(dot_pos + 1);
		return (extension);
	}
}

std::string	Response::getDate() const
{
	char	buffer[30];
	buffer[29] = '\0';
	time_t timestamp;
	time(&timestamp);

	struct tm datetime = *localtime(&timestamp);
	strftime(buffer, 30, "%a, %d %b %G %T GMT", &datetime);
	std::string result = buffer;
	return (result);
}

const std::string	Response::getSerializedHeaders() const
{
	std::string result;
	std::vector<std::pair<std::string, std::string> >::const_iterator it;
	for(it = _headers.begin(); it != _headers.end(); it++)
	{
		result += it->first + ": " + it->second + "\r\n";
	}
	return result;
}

//Need to check if adding header date here is ok
const std::string	Response::getSerializedResponse()
{
	std::string response;
	addHeader("date", getDate());
	response += "HTTP/";
	response += toString(this->_http_version);
	response += " ";
	response += toString(this->_status_code) + " " + this->_status_name + "\r\n";
	response += getSerializedHeaders() + "\r\n" + getBody() + "\n";
	return (response);
}

void	Response::defineContentType()
{
	std::string extension = getExtension(this->getRessourcePath());
	if (extension.empty())
		throw InternalServerErrorException();
	else
	{
		std::map<std::string, std::string>::const_iterator it;
		it = _content_types.find(extension);
		if (it != _content_types.end())
			addHeader("content-type", it->second);
		else
			addHeader("content-type", "application/octet-stream");
	}
}

Response::Response() : Message(), _status_code(0)
{
	this->initBuilders();
}

Response::Response(const int status_code, const std::string status_name)
					: Message(), _status_code(status_code), _status_name(status_name)
{
	this->initBuilders();
	this->process();
}

Response::Response(const Response &copy) : Message(copy)
{
	this->initBuilders();
	*this = copy;
}

Response::~Response() {}

Response&	Response::operator=(const Response& src)
{
	if (this != &src)
	{
		Message::operator=(src);
		this->_status_code = src._status_code;
		this->_status_name = src._status_name;
	}
	return (*this);
}
