/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:15:13 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/18 11:41:43 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

std::string get_first_word(std::string &message) {
	std::string::size_type start = message.find_first_not_of(" \t\n\r");
	if (start == std::string::npos)
		return "";
	std::string::size_type end = message.find_first_of(" \t\n\r", start);
	std::string result = message.substr(start, end - start);
	if (end != std::string::npos)
		message = message.substr(end);
	else
		message.clear();
	return result;
}

double str_to_double(const std::string &s)
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

double	get_http_version(std::string &message) {
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

std::string	find_ressource_path(const std::string& uri)
{
	std::string result;
	
	size_t pos = uri.find('?');
	if (pos == std::string::npos)
	{
		result = uri;
		return (result);
	}
	else
	{
		result = uri.substr(0, pos);
		return (result);
	}
}

std::string	find_query_string(const std::string& uri)
{
	std::string result;
	
	size_t pos = uri.find('?');
	result = (pos != std::string::npos) ? uri.substr(pos + 1) : "";
	return (result);
}

std::string get_next_line(std::string &message)
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