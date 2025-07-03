/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handle_Method.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:33:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/03 11:18:14 by pmateo           ###   ########.fr       */
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
	Response* response = NULL;
	try
	{
		body = get_file_body(request);
		response = new Response(200, "OK");
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

Response*	HandlePOST(const Request& request __attribute_maybe_unused__)
{
	return (NULL);
}

Response*	HandleDELETE(const Request& request __attribute_maybe_unused__)
{
	return (NULL);
}