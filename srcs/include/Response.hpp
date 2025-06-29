/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:57:23 by pmateo            #+#    #+#             */
/*   Updated: 2025/06/29 17:11:26 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Message.hpp"
#include <map>

#define RESPONSE_NB 15

class Response : public Message
{
	private:
		int _status;
		typedef void (*ResponseFunction)();
		static std::map<int, ResponseFunction> builders;
	
	public:
		Response(int status);
		Response(const Response &copy);
		~Response();
		Response 		&operator=(const Response &src);

		void			process();

		void			setStatus(const std::string status);
		
		std::string		getStatus() const;
		const char *	getSerializedResponse() const;

		//STATUS FUNCTIONS
		//2xx : SUCCESS RESPONSE
		void	Ok();
		void	Created();
		void	Accepted();
		
		//3xx : REDIRECTION RESPONSE
		void	MovedPermanently();
		
		//4xx : CLIENT ERROR RESPONSE
		void	BadRequest();
		void	Forbidden();
		void	NotFound();
		void	LengthRequired();
		void	UriTooLong();
		void	ImATeapot();
		void	TooManyRequest();

		//5xx : SERVER ERROR RESPONSE
		void	InternalServerError();
		void	NotImplemented();
		void	ServiceUnavailable();
		void	HttpVersionNotSupported();

		static	void	initBuilders();

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