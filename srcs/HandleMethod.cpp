/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:33:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/04 19:01:30 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

Response*	HandleHEAD(ServerConfig conf __attribute_maybe_unused__, const Request& request __attribute_maybe_unused__)
{
	return (NULL);
}

Response*	HandleGET(ServerConfig conf, const Request& request)
{
	std::string body;
	Response* response = NULL;
	try
	{
		if (request.getUri() == "/")
			body = get_file_content(conf.getRoot() + conf.getIndex());
		else
			body = get_file_content(conf.getRoot() + request.getUri());
		response = new Response(200, "OK");
		// response->FindContentType(request.getUri());
		response->setBody(body);
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	catch(const Response::ResourceNotFoundException& e)
	{
		response = new Response(404, "Not Found");
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	catch(const Response::InternalServerErrorException& e)
	{
		response = new Response(500, "Internal Server Error");
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	return (response);
}

Response*	HandlePOST(ServerConfig conf __attribute_maybe_unused__, const Request& request __attribute_maybe_unused__)
{
	std::string body;
	Response* response = NULL;
	try
	{
		if (request.getUri() == "/stopserv")
			body = get_file_content(conf.getRoot() + conf.getStopServer());
		
		else
			body = get_file_content(conf.getRoot() + request.getUri());

		response = new Response(200, "OK");

		// response->FindContentType(request.getUri());
		response->setBody(body);
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	catch(const Response::ResourceNotFoundException& e)
	{
		response = new Response(404, "Not Found");
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	catch(const Response::InternalServerErrorException& e)
	{
		response = new Response(500, "Internal Server Error");
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	return (response);
}

Response*	HandleDELETE(ServerConfig conf __attribute_maybe_unused__, const Request& request __attribute_maybe_unused__)
{
	return (NULL);
}