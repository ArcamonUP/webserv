/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:57:23 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/09 16:42:57 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Message.hpp"

class Response : public Message
{
	private:
		int _status_code;
		std::string _status_name;
		std::string	_ressource_path;
		std::string	_error_json;
		typedef void (Response::*ResponseFunction)();
		static std::map<int, ResponseFunction> _builders;
		static std::map<std::string, std::string> _content_types;
	
	public:
		Response();
		Response(const int status_code, const std::string status_name);
		Response(const Response &copy);
		~Response();
		Response 			&operator=(const Response &src);

		void				process();

		void				setStatusCode(const int status_code);
		void				setStatusName(const std::string status_name);
		void				setRessourcePath(const std::string requested_ressource_path);
		void				setContentLength(const std::string length);
		void				setContentType(const std::string type);
		void				setDate(); // Using getDate()
		void				setLocation(const std::string location);
		
		int					getStatusCode() const;
		std::string			getStatusName() const;
		std::string			getRessourcePath() const;
		std::string 		getDate() const; //Get UTC by strftime()
		std::string			getExtension(const std::string& Uri) const; 
		const std::string	getSerializedHeaders() const;
		const std::string	getSerializedResponse();

		void				defineContentType();
		void		FindContentType(const std::string& Uri);
		std::string 		createJsonError(const std::string& error, const std::string& message);

		static	void		initBuilders();
		static	void		initContentTypes();

		//BUILDERS STATUS FUNCTIONS
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
		void	BadGateway();				//502
		void	ServiceUnavailable();		//503
		void	GatewayTimeout();			//504
		void	HttpVersionNotSupported();	//505

		class ResourceForbiddenException : public std::exception {};
		class ResourceNotFoundException : public std::exception {};
		class InternalServerErrorException : public std::exception {};
};

inline std::ostream &operator<<(std::ostream &stream, Response const &response) {
	stream << "--> Status: " << response.getStatusCode() << " " << response.getStatusName() << std::endl << \
	"--> Http Version: " << response.getHttpVersion() << std::endl << \
	"--> Headers/Value: " << std::endl << response.getHeaderMap() << \
	"--> Body: " << response.getBody() << std::endl << \
	"--> Error: " << response.getError() << std::endl;
			return (stream);
}