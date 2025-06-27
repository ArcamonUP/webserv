/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:06:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/27 18:36:45 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <exception>
#include <sstream>

class Message
{		 
	protected:
		Message();
		Message(const Message &copy);

		std::string	_uri;
		double		_http_version;
		std::vector<std::pair<std::string, std::string> > _headers;
		std::string	_body;
		bool		_error;
	
	public:
		virtual ~Message();
		Message	&operator=(const Message &src);

		virtual void	process() = 0;

		void 			setUri(const std::string uri);
		void			setHttpVersion(const double http_version);
		void			setHeaderMap(const std::string headermap);
		void			setHeaderValue(std::string key, std::string value);
		void			setBody(const std::string body);
		void			setError(const bool error);

		std::string		getUri() const;
		double			getHttpVersion() const;
		std::string		getHeaderMap() const;
		std::string		getHeaderValue(std::string key) const;
		std::string		getBody() const;
		bool			getError() const;
};