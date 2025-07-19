/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseStatus.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:37:59 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/19 16:52:27 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

void	Response::Ok()
{
	this->_status_code = 200;
	this->_status_name = "OK";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "keep-alive");
}

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
	addHeader("content-type", "text/html ; charset=utf-8");
	body = "Moved Permanently. Redirecting to " + location + "\n";
	setBody(body);
}

void	Response::BadRequest()
{
	this->_status_code = 400;
	this->_status_name = "Bad Request";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
	addHeader("content-length", toString(getBody().length()));
}

void	Response::Forbidden()
{
	this->_status_code = 403;
	this->_status_name = "Forbidden";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
	addHeader("content-length", toString(getBody().length()));
}

void	Response::NotFound()
{
	this->_status_code = 404;
	this->_status_name = "Not Found";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
	std::string body = "<!doctype html>\n";
	body += "<head>\n	<title>404 not found !</title>";
	setBody(body);
}

void	Response::MethodNotAllowed()
{
	this->_status_code = 405;
	this->_status_name = "Method Not Allowed";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
	addHeader("allow", "GET, POST, HEAD, DELETE");
}

void	Response::LengthRequired()
{
	this->_status_code = 411;
	this->_status_name = "Length Required";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
	addHeader("content-length", toString(getBody().length()));
}

void	Response::PayLoadTooLarge()
{
	this->_status_code = 413;
	this->_status_name = "Payload Too Large";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
	addHeader("content-length", toString(getBody().length()));
}

void	Response::UriTooLong()
{
	this->_status_code = 414;
	this->_status_name = "URI Too Long";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
	addHeader("content-length", toString(getBody().length()));
}

void	Response::ImATeapot()
{
	this->_status_code = 418;
	this->_status_name = "I'm a teapot";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
	addHeader("content-length", toString(getBody().length()));
}

void	Response::TooManyRequest()
{
	this->_status_code = 429;
	this->_status_name = "Too Many Request";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("retry-after", "1800");
	addHeader("content-type", "text/html ; charset=utf-8");
	addHeader("content-length", toString(getBody().length()));
}

void	Response::InternalServerError()
{
	this->_status_code = 500;
	this->_status_name = "Internal Server Error";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
	std::string body = "<!doctype html>\n";
	body += "<head>\n	<title>500 Internal Server Error !</title>";
	setBody(body);
}

void	Response::NotImplemented()
{
	this->_status_code = 501;
	this->_status_name = "Not Implemented";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
}

void	Response::BadGateway()
{
	this->_status_code = 502;
	this->_status_name = "Bad Gateway";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
	std::string body = "<!doctype html>\n";
	body += "<head>\n	<title>502 Bad Gateway</title>";
	setBody(body);
}

void	Response::ServiceUnavailable()
{
	this->_status_code = 503;
	this->_status_name = "Service Unavailable";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
	std::string body = "<!doctype html>\n";
	body += "<head>\n	<title>503 Service Unavailable</title>";
	setBody(body);
}

void	Response::GatewayTimeout()
{
	this->_status_code = 504;
	this->_status_name = "Gateway Timeout";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
	std::string body = "<!doctype html>\n";
	body += "<head>\n	<title>504 Gateway Timeout</title>";
	setBody(body);
}

void	Response::HttpVersionNotSupported()
{
	this->_status_code = 505;
	this->_status_name = "HTTP Version Not Supported";
	addHeader("server", "42WebServ/1.0");
	addHeader("connection", "close");
	addHeader("content-type", "text/html ; charset=utf-8");
	std::string body = "<!doctype html>\n";
	body += "<head>\n	<title>505 HTTP Version Not Supported</title>";
	setBody(body);
}
