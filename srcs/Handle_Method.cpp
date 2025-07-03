/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handle_Method.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:33:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/03 03:09:57 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Response*	HandleHEAD(const Request& request __attribute_maybe_unused__)
{
	return (NULL);
}

Response*	HandleGET(const Request& request)
{
	std::string body;
	Response* response;
	std::cout << "badibou" << std::endl;
	try
	{
		body = get_file_body(request);
		std::cout << "euuuuuh" << std::endl;
		response = new Response(200, "OK");
		std::cout << "wtf" << std::endl;
		response->setBody(body);
		response->process();
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	catch(const Response::ResourceNotFoundException& e)
	{
		std::cout << "atouchm" << std::endl;
		response = new Response(404, "Not Found");
		response->process();
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	catch(const Response::InternalServerErrorException& e)
	{
		std::cout << "jcapte pas" << std::endl;
		response = new Response(500, "Internal Server Error");
		response->process();
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	return (response);
}

Response*	HandlePOST(const Request& request __attribute_maybe_unused__)
{
	return (NULL);
}

Response*	HandleDELETE(const Request& request __attribute_maybe_unused__)
{
	return (NULL);
}