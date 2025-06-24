/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:12:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/24 16:07:17 by kbaridon         ###   ########.fr       */
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

static double	get_http_version(std::string &request) {
	std::string 			temp;
	std::string::size_type	l;
	double					result;

	temp = get_first_word(request);
	l = temp.find_first_of("/");
	temp = temp.substr(l + 1);
	result = std::atof(temp.c_str());
	return (result);	
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
}

Request::Request(const Request &copy) {
	*this = copy;
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
