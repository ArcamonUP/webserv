/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:06:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/26 01:12:04 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <exception>
#include <sstream>

#define REQUEST 1
#define RESPONSE 2

class Message
{
	private:
		const int	_type;
		std::string _method_or_status;
		std::string	_uri;
		double		_http_version;
		std::vector<std::pair<std::string, std::string> > _headers;
		std::string	_body;
		bool		_error;
	public:
		Message();
		Message(std::string message);
		Message(const Message &copy);
		~Message();
		Message	&operator=(const Message &src);

		const char *	buildResponse() const;

		void			setMethodOrStatus(const std::string method);
		void 			setUri(const std::string uri);
		void			setHttpVersion(const double http_version);
		void			setHeaderMap(const std::string headermap);
		void			setHeaderValue(std::string key, std::string value);
		void			setBody(const std::string body);
		void			setError(const bool error);

		int				getType() const;
		std::string		getMethodOrStatus() const;
		std::string		getUri() const;
		double			getHttpVersion() const;
		std::string		getHeaderMap() const;
		std::string		getHeaderValue(std::string key) const;
		std::string		getBody() const;
		bool			getError() const;

		class ResourceNotFoundException : public std::exception {};
};

std::ostream &operator<<(std::ostream &stream, Message const &message);
