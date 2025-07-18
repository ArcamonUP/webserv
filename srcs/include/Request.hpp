/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:42:58 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/18 11:47:24 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Message.hpp"

class Request : public Message
{
	private:
		std::string _method;
		std::string	_uri;
		std::string _ressource_path;
		std::string	_query_string;
		std::string	_serialized_request;
		int		sys_call_verif;
	
	public:
		Request(std::string serialized_request);
		Request(const Request& copy);
		~Request();
		Request &operator=(const Request& src);

		void				process();

		void				setUri(std::string uri);

		const std::string&	getSerializedRequest() const;
		const std::string&	getMethod() const;
		const std::string&	getUri() const;
		const std::string&	getRessourcePath() const;
		const std::string&	getQueryString() const;
		int					getSysCallVerif() const;

		void				setSysCallVerif(int i);

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


//fonctions non-membres ?
std::string 	get_first_word(std::string &message);
double 			str_to_double(const std::string &s);
double			get_http_version(std::string &message);
std::string		find_ressource_path(const std::string& uri);
std::string		find_query_string(const std::string& uri);
std::string 	get_next_line(std::string &message);