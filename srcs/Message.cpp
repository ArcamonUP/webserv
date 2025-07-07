/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:12:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/04 16:53:24 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

Message::Message() : _http_version(1.1) {}

Message::Message(const Message &copy)
{
	*this = copy;
}

Message::~Message() {}

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
//Si header non trouve ? 
void	Message::setHeaderValue(std::string key, std::string value)
{
	std::vector<std::pair<std::string, std::string> >::iterator it;
	for (it = this->_headers.begin(); it != this->_headers.end(); ++it)
	{
		if (it->first == key)
			it->second = value;
	}
}

void	Message::addHeader(const std::string first, const std::string second)
{
	_headers.push_back(std::make_pair(first, second));
}

void	Message::setBody(const std::string body)
{
	this->_body = body;
	std::string length_str = toString(body.length());
	
	std::string ContentLength = getHeaderValue("content-length");
	if (ContentLength.empty())
		addHeader("content-length", length_str);
	else
		setHeaderValue("content-length", length_str);
}

void	Message::setError(const bool error)
{
	this->_error = error;
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
    return "";
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
		this->_http_version = src._http_version;
		this->_error = src._error;
	}
	return (*this);
}
