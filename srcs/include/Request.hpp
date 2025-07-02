/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:42:58 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/02 21:16:52 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Message.hpp"

class Request : public Message
{
	private:
		std::string _method;
		std::string	_uri;
		std::string	_serialized_request;
	
	public:
		Request(std::string serialized_request);
		Request(const Request& copy);
		~Request();
		Request &operator=(const Request& src);

		void			process();

		std::string		getSerializedRequest() const;
		std::string		getMethod() const;
		std::string		getUri() const;
		double			getHttpVersion() const;
		std::string		getHeaderMap() const;
		std::string		getHeaderValue(std::string key) const;
		std::string		getBody() const;
		bool			getError() const;

};

inline std::ostream &operator<<(std::ostream &stream, Request const &request)
{
	stream << "--> Method: " << request.getMethod() << std::endl << \
	"--> Uri: " << request.getUri() << std::endl << \
	"--> Http Version: " << request.getHttpVersion() << std::endl << \
	"--> Headers/Value: " << std::endl << request.getHeaderMap() << \
	"--> Body: " << request.getBody() << std::endl << \
	"--> Error: " << request.getError() << std::endl;
			return (stream);
}

std::string 	get_first_word(std::string &message);
double 			str_to_double(const std::string &s);
double			get_http_version(std::string &message);
std::string		trim(std::string str);
std::string 	get_next_line(std::string &message);