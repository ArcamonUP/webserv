/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:57:23 by pmateo            #+#    #+#             */
/*   Updated: 2025/06/27 19:26:41 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Message.hpp"

class Response : public Message
{
	private:
		std::string _status;
	
	public:
		Response();
		Response(const Response &copy);
		~Response();
		Response 		&operator=(const Response &src);

		void			process();

		void			setStatus(const std::string status);
		
		std::string		getStatus() const;
		const char *	getSerializedResponse() const;

		class ResourceNotFoundException : public std::exception {};
};

inline std::ostream &operator<<(std::ostream &stream, Response const &response) {
	stream << "--> Method: " << response.getStatus() << std::endl << \
	"--> Uri: " << response.getUri() << std::endl << \
	"--> Http Version: " << response.getHttpVersion() << std::endl << \
	"--> Headers/Value: " << std::endl << response.getHeaderMap() << \
	"--> Body: " << response.getBody() << std::endl << \
	"--> Error: " << response.getError() << std::endl;
			return (stream);
}