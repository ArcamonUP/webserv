/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:06:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/04 16:53:24 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "WebServ.hpp"

class Message
{		 
	protected:
		Message();
		Message(const Message &copy);

		double		_http_version;
		std::vector<std::pair<std::string, std::string> > _headers;
		std::string	_body;
		bool		_error;
	
	public:
		virtual ~Message();
		Message	&operator=(const Message &src);

		virtual void	process() = 0;

		void			setHeaderMap(const std::string headermap);
		void			setHeaderValue(const std::string key, const std::string value);
		void			addHeader(const std::string first, const std::string second);
		void			setBody(const std::string body);
		void			setError(const bool error);
		double			getHttpVersion() const;
		std::string		getHeaderMap() const;
		std::string		getHeaderValue(std::string key) const;
		std::string		getBody() const;
		bool			getError() const;
};