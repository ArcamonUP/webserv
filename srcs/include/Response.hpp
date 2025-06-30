/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:57:23 by pmateo            #+#    #+#             */
/*   Updated: 2025/06/30 23:18:58 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Message.hpp"
#include <map>

class Response : public Message
{
	private:
		int _status_code = 0;
		std::string _status_name;
		typedef void (Response::*ResponseFunction)();
		static std::map<int, ResponseFunction> _builders;
	
	public:
		Response();
		Response(const int status_code, const std::string status_name);
		Response(const Response &copy);
		~Response();
		Response 			&operator=(const Response &src);

		void				process();

		void				setStatusCode(const int status_code);
		void				setStatusName(const std::string status_name);
		
		int					getStatusCode() const;
		int					getStatusName() const;
		const std::string	getSerializedHeaders() const;
		const char *		getSerializedResponse() const;

		static	void		initBuilders();

		//STATUS FUNCTIONS
		//2xx : SUCCESS RESPONSE
		void	Ok();						//200
		void	Created();					//201
		void	Accepted();					//202
		
		//3xx : REDIRECTION RESPONSE
		void	MovedPermanently(); 		//301
		
		//4xx : CLIENT ERROR RESPONSE
		void	BadRequest();				//400
		void	Forbidden();				//403
		void	NotFound();					//404
		void	MethodNotAllowed();			//405
		void	LengthRequired();			//411
		void	UriTooLong();				//414
		void	ImATeapot();				//418
		void	TooManyRequest();			//429

		//5xx : SERVER ERROR RESPONSE
		void	InternalServerError();		//500
		void	NotImplemented();			//501
		void	ServiceUnavailable();		//503
		void	HttpVersionNotSupported();	//504

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