/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:12:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/25 16:10:05 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

static std::string get_first_word(std::string &request) {
	std::string::size_type start = request.find_first_not_of(" \t\n\r");
	if (start == std::string::npos)
		return "";
	std::string::size_type end = request.find_first_of(" \t\n\r", start);
	std::string result = request.substr(start, end - start);
	if (end != std::string::npos)
		request = request.substr(end);
	else
		request.clear();
	if (result == "/")
		return ("");
	return result;
}

static double str_to_double(const std::string &s)
{
	double value = 0.0;
	double div   = 1.0;
	bool afterDot = false;

	for (std::size_t i = 0; i < s.size(); ++i)
	{
		char c = s[i];
		if (c == '.')
		{
			if (afterDot)
				break ;
			afterDot = true;
			continue;
		}
		if (c < '0' || c > '9')
			break;
		int digit = c - '0';
		if (!afterDot) {
			value = value * 10 + digit;
		} else {
			div *= 10;
			value += digit / div;
		}
	}
	return value;
}


static double	get_http_version(std::string &request) {
	std::string 			temp;
	std::string::size_type	l;
	double					result;

	temp = get_first_word(request);
	if (temp.empty())
		return (0);
	l = temp.find_first_of("/");
	if (l == std::string::npos)
		return (0);
	temp = temp.substr(l + 1);
	result = str_to_double(temp);
	return (result);	
}

static std::string	trim(std::string str) {
	size_t start = str.find_first_not_of(" \t\n\r");
	size_t end = str.find_last_not_of(" \t\n\r");
	std::string	result;

	if (start == std::string::npos)
		result.clear();
	else
		result = str.substr(start, end - start + 1);
	return (result);
}

static std::string get_next_line(std::string &request)
{
    size_t end = request.find("\r\n");
    std::string line;
    if (end != std::string::npos) {
        line = request.substr(0, end);
        request = request.substr(end + 2);
    }
    else
    {
        line = request;
        request.clear();
    }
    return line;
}

Request::Request() {}
Request::~Request() {}

Request::Request(std::string request)
{
	this->method = get_first_word(request);
	this->uri = get_first_word(request);
	this->http_version = get_http_version(request);
	if (this->method.empty() || this->uri.empty() || this->http_version == 0)
		this->error = true;
	else
		this->error = false;
	request = request.substr(request.find_first_not_of(" \t\n\r"));
	std::string line = get_next_line(request);
	while (!line.empty() && line != "\r\n")
	{
		size_t mid = line.find(':');
		if (mid == std::string::npos) {
			line = get_next_line(request);
			continue;
		}
		std::string key = trim(line.substr(0, mid));
		std::string value = trim(line.substr(mid + 1));
		headers.push_back(std::make_pair(key, value));
		line = get_next_line(request);
	}
	this->body.swap(request);
}

Request::Request(const Request &copy) {
	*this = copy;
}

std::string	Request::getMethod() const {
	return (this->method);
}

std::string	Request::getUri() const {
	return (this->uri);
}

double	Request::getHttpVersion() const {
	return (this->http_version);
}

std::string	Request::getHeaderMap() const {
	std::string result;
	for (size_t i = 0; i < this->headers.size(); ++i)
		result += this->headers[i].first + ": " + this->headers[i].second + "\n";
	if (result.empty())
		return "No headers.\n";
	return result;
}


std::string Request::getHeaderValue(std::string key) const {
	std::vector<std::pair<std::string, std::string> >::const_iterator it;

	it = this->headers.begin();
    for (; it != this->headers.end(); ++it) {
        if (it->first == key)
            return it->second;
    }
    return "Not Found.";
}


std::string	Request::getBody() const {
	return (this->body);
}

bool	Request::getError() const {
	return (this->error);
}

Request &Request::operator=(const Request &src) {
	if (this != &src)
	{
		this->method = src.method;
		this->uri = src.uri;
		this->http_version = src.http_version;
		this->error = src.error;
	}
	return (*this);
}

std::ostream &operator<<(std::ostream &stream, Request const &request) {
	stream << "--> Method: " << request.getMethod() << std::endl << \
	"--> Uri: " << request.getUri() << std::endl << \
	"--> Http Version: " << request.getHttpVersion() << std::endl << \
	"--> Headers/Value: " << std::endl << request.getHeaderMap() << \
	"--> Body: " << request.getBody() << std::endl << \
	"--> Error: " << request.getError() << std::endl;
	return (stream);
}
