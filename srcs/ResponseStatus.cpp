/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseStatus.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:37:59 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/04 18:37:03 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

//For 200, 201, 202 : Body, Content-Length and Content-Type need to be set upstream
//Date header need to be set just before the response is send to the client

//No Content is missing

//For the moment we assume the content-type is html but we need to define it
//The content length need to be set after the body has been recovered
void	Response::Ok()
{
	this->_status_code = 200;
	this->_status_name = "OK";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "keep-alive");
}
//201 need a "location" header
void	Response::Created()
{
	this->_status_code = 201;
	this->_status_name = "Created";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "keep-alive");
}

void	Response::Accepted()
{
	this->_status_code = 202;
	this->_status_name = "Accepted";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "keep-alive");
}
//301 need a "location" header
void	Response::MovedPermanently()
{
	std::string body;
	std::string location = getHeaderValue("location");
	if (location.empty())
		(this->*_builders[500])();
	this->_status_code = 301;
	this->_status_name = "Moved Permanently";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "keep-alive");
	addHeader("content-type", "text/plain ; charset=utf-8");
	body = "Moved Permanently. Redirecting to " + location + "\n";
	setBody(body);
	// addHeader("content-length", toString(getBody().length()));
}

void	Response::BadRequest()
{
	this->_status_code = 400;
	this->_status_name = "Bad Request";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "application/json");
	std::string error = "Bad request";
	std::string message = "Request body could not be read properly.";
	setBody(createJsonError(error, message));
	addHeader("content-length", toString(getBody().length()));
}
//Generic response is certainly insufficient
void	Response::Forbidden()
{
	this->_status_code = 403;
	this->_status_name = "Forbidden";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "application/json");
	std::string error = "InsufficientPermissions";
	std::string message = "Insufficient permissions for this request.";
	setBody(createJsonError(error, message));
	addHeader("content-length", toString(getBody().length()));
}

void	Response::NotFound()
{
	this->_status_code = 404;
	this->_status_name = "Not Found";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html");
	std::string body = "<!doctype html>\n";
	body += "<head>\n	<title>404 not found !</title>";
	setBody(body);
	// addHeader("content-length", toString(body.length()));
}
//Allow header depends ??
void	Response::MethodNotAllowed()
{
	this->_status_code = 405;
	this->_status_name = "Method Not Allowed";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("allow", "GET, POST, HEAD, DELETE");
}

void	Response::LengthRequired()
{
	this->_status_code = 411;
	this->_status_name = "Length Required";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "application/json");
	std::string error = "LengthRequired";
	std::string message = "Requests must have a content length header";
	setBody(createJsonError(error, message));
	addHeader("content-length", toString(getBody().length()));
}

void	Response::UriTooLong()
{
	this->_status_code = 414;
	this->_status_name = "URI Too Long";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "application/json");
	std::string error = "URI Too Long";
	std::string message = "The URI provided was too long for the server to process";
	setBody(createJsonError(error, message));
	addHeader("content-length", toString(getBody().length()));
}

void	Response::ImATeapot()
{
	this->_status_code = 418;
	this->_status_name = "I'm a teapot";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "application/json");
	std::string error = "I'mATeapot";
	std::string message = "Seriously ? Did u just tried to make a gad damn coffee with a fucking teapot ?";
	setBody(createJsonError(error, message));
	addHeader("content-length", toString(getBody().length()));
}
//Need a "Retry-After" header 
void	Response::TooManyRequest()
{
	this->_status_code = 429;
	this->_status_name = "Too Many Request";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "application/json");
	addHeader("retry-after", "1800");
	std::string error = "TooManyRequest";
	std::string message = "You're doing that too often ! Are you trying to crash this server ? Try again later.";
	setBody(createJsonError(error, message));
	addHeader("content-length", toString(getBody().length()));
}

void	Response::InternalServerError()
{
	this->_status_code = 500;
	this->_status_name = "Internal Server Error";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html");
	std::string body = "<!doctype html>\n";
	body += "<head>\n	<title>500 Internal Server Error !</title>";
	setBody(body);
	// addHeader("content-length", toString(getBody().length()));
}

void	Response::NotImplemented()
{
	this->_status_code = 501;
	this->_status_name = "Not Implemented";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
}
//Should have a "Retry-After" header
void	Response::ServiceUnavailable()
{
	this->_status_code = 503;
	this->_status_name = "Service Unavailable";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html");
	std::string body = "<!doctype html>\n";
	body += "<head>\n	<title>503 Service Unavailable</title>";
	setBody(body);
	// addHeader("content-length", toString(getBody().length()));
}

void	Response::HttpVersionNotSupported()
{
	this->_status_code = 505;
	this->_status_name = "HTTP Version Not Supported";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html");
	std::string body = "<!doctype html>\n";
	body += "<head>\n	<title>505 HTTP Version Not Supported</title>";
	setBody(body);
	// addHeader("content-length", toString(getBody().length()));
}
