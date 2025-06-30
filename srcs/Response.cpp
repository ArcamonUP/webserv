/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:02:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/06/30 23:19:56 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void	Response::process()
{
	try
	{
		(this->*_builders.at(this->_status))();
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

int	Response::getStatusCode() const
{
	return (this->_status_code);
}

int	Response::getStatusName() const
{
	return (this->_status_name);
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

const char *	Response::getSerializedResponse() const
{
	std::string response;
	response +=
}

void	Response::initBuilders()
{
	Response::_builders[200] = &Ok;
	Response::_builders[201] = &Created;
	Response::_builders[202] = &Accepted;
	Response::_builders[301] = &MovedPermanently;
	Response::_builders[400] = &BadRequest;
	Response::_builders[403] = &Forbidden;
	Response::_builders[404] = &NotFound;
	Response::_builders[405] = &MethodNotAllowed;
	Response::_builders[411] = &LengthRequired;
	Response::_builders[414] = &UriTooLong;
	Response::_builders[418] = &ImATeapot;
	Response::_builders[429] = &TooManyRequest;
	Response::_builders[500] = &InternalServerError;
	Response::_builders[501] = &NotImplemented;
	Response::_builders[503] = &ServiceUnavailable;
	Response::_builders[504] = &HttpVersionNotSupported;
}

Response::Response() {}

Response::Response(const int status_code, const std::string status_name)
					: _status_code(status_code), _status_name(status_name)
{
	this->process();
}

Response::Response(const Response &copy)
{
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