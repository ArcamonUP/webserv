/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:12:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/26 01:34:03 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

static std::string get_first_word(std::string &message) {
	std::string::size_type start = message.find_first_not_of(" \t\n\r");
	if (start == std::string::npos)
		return "";
	std::string::size_type end = message.find_first_of(" \t\n\r", start);
	std::string result = message.substr(start, end - start);
	if (end != std::string::npos)
		message = message.substr(end);
	else
		message.clear();
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


static double	get_http_version(std::string &message) {
	std::string 			temp;
	std::string::size_type	l;
	double					result;

	temp = get_first_word(message);
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

static std::string get_next_line(std::string &message)
{
    size_t end = message.find("\r\n");
    std::string line;
    if (end != std::string::npos) {
        line = message.substr(0, end);
        message = message.substr(end + 2);
    }
    else
    {
        line = message;
        message.clear();
    }
    return line;
}

Message::Message() : _type(RESPONSE) {}
Message::~Message() {}

Message::Message(std::string request) : _type(REQUEST)
{
	this->_method_or_status = get_first_word(request);
	this->_uri = get_first_word(request);
	this->_http_version = get_http_version(request);
	if (this->_method_or_status.empty() || this->_uri.empty() || this->_http_version == 0)
		this->_error = true;
	else
		this->_error = false;
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
		_headers.push_back(std::make_pair(key, value));
		line = get_next_line(request);
	}
	this->_body.swap(request);
}

Message::Message(const Message &copy) : _type(copy._type) {
	*this = copy;
}

void	Message::setMethodOrStatus(const std::string method)
{
	this->_method_or_status = method;
}

void	Message::setUri(const std::string uri)
{
	this->_uri = uri;
}

void	Message::setHttpVersion(const double http_version)
{
	this->_http_version = http_version;
}

void	Message::setHeaderMap(const std::string __attribute_maybe_unused__ headermap)
{
	// while (!headermap.empty() && headermap != "\r\n")
	// {
	// 	size_t mid = headermap.find(':');
	// 	if (mid == std::string::npos) {
	// 		line = get_next_line(request);
	// 		continue;
	// 	}
	// 	std::string key = trim(line.substr(0, mid));
	// 	std::string value = trim(line.substr(mid + 1));
	// 	_headers.push_back(std::make_pair(key, value));+
	// 	line = get_next_line(request);
	// }
}

void	Message::setHeaderValue(std::string __attribute_maybe_unused__ key, std::string __attribute_maybe_unused__ value)
{
	// this->_headers[key] = value;
}

void	Message::setBody(const std::string body)
{
	this->_body = body;
}

void	Message::setError(const bool error)
{
	this->_error = error;
}

std::string	Message::getMethodOrStatus() const {
	return (this->_method_or_status);
}

std::string	Message::getUri() const {
	return (this->_uri);
}

double	Message::getHttpVersion() const {
	return (this->_http_version);
}

std::string	Message::getHeaderMap() const {
	std::string result;
	for (size_t i = 0; i < this->_headers.size(); ++i)
		result += this->_headers[i].first + ": " + this->_headers[i].second + "\n";
	if (result.empty())
		return "No headers.\n";
	return result;
}


std::string Message::getHeaderValue(std::string key) const {
	std::vector<std::pair<std::string, std::string> >::const_iterator it;

	it = this->_headers.begin();
    for (; it != this->_headers.end(); ++it) {
        if (it->first == key)
            return it->second;
    }
    return "Not Found.";
}


std::string	Message::getBody() const {
	return (this->_body);
}

bool	Message::getError() const {
	return (this->_error);
}

Message &Message::operator=(const Message &src) {
	if (this != &src)
	{
		this->_method_or_status = src._method_or_status;
		this->_uri = src._uri;
		this->_http_version = src._http_version;
		this->_error = src._error;
	}
	return (*this);
}

std::ostream &operator<<(std::ostream &stream, Message const &request) {
	stream << "--> Method: " << request.getMethodOrStatus() << std::endl << \
	"--> Uri: " << request.getUri() << std::endl << \
	"--> Http Version: " << request.getHttpVersion() << std::endl << \
	"--> Headers/Value: " << std::endl << request.getHeaderMap() << \
	"--> Body: " << request.getBody() << std::endl << \
	"--> Error: " << request.getError() << std::endl;
	return (stream);
}
