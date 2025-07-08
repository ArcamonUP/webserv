/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:33:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/08 14:40:03 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <sys/stat.h>

Response*	HandleHEAD(ServerConfig conf __attribute_maybe_unused__, const Request& request __attribute_maybe_unused__)
{
	return (NULL);
}

Response*	HandleGET(ServerConfig conf, const Request& request)
{
	std::string body, file_path;
	Response* response = NULL;
	try
	{
		std::string uri = request.getUri();
		int location_index = find_matching_location_index(conf, uri);
		
		if (uri == "/")
			file_path = conf.getRoot() + conf.getIndex();
		else
			file_path = conf.getRoot() + uri;
		struct stat path_stat;
		if (stat(file_path.c_str(), &path_stat) == 0) {
			if (S_ISDIR(path_stat.st_mode)) {
				std::string index_file = file_path;
				if (file_path.empty() || file_path[file_path.length() - 1] != '/') index_file += "/";
				
				std::string index_name = "index.html";
				if (location_index >= 0 && !conf.getLocations()[location_index].getIndex().empty()) {
					index_name = conf.getLocations()[location_index].getIndex();
				}
				index_file += index_name;
				
				if (access(index_file.c_str(), F_OK) == 0)
					body = get_file_content(index_file);
				else
				{
					bool autoindex_enabled = false;
					if (location_index >= 0) {
						autoindex_enabled = conf.getLocations()[location_index].getAutoIndex();
					}
					
					if (autoindex_enabled)
						body = generate_autoindex(file_path, uri);
					else
					{
						response = new Response(403, "Forbidden");
						std::string error_body = get_custom_error_page(conf, 403);
						response->setBody(error_body);
						return response;
					}
				}
			}
			else
				body = get_file_content(file_path);
		}
		else
			throw Response::ResourceNotFoundException();
		response = new Response(200, "OK");
		response->setBody(body);
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	catch(const Response::ResourceNotFoundException& e)
	{
		response = new Response(404, "Not Found");
		std::string error_body = get_custom_error_page(conf, 404);
		response->setBody(error_body);
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	catch(const Response::InternalServerErrorException& e)
	{
		response = new Response(500, "Internal Server Error");
		std::string error_body = get_custom_error_page(conf, 500);
		response->setBody(error_body);
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
		response->setBody(body);
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	catch(const Response::ResourceNotFoundException& e)
	{
		response = new Response(404, "Not Found");
		std::string error_body = get_custom_error_page(conf, 404);
		response->setBody(error_body);
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	catch(const Response::InternalServerErrorException& e)
	{
		response = new Response(500, "Internal Server Error");
		std::string error_body = get_custom_error_page(conf, 500);
		response->setBody(error_body);
		std::cout << "ser_response : " << response->getSerializedResponse();
	}
	return (response);
}

Response*	HandleDELETE(ServerConfig conf __attribute_maybe_unused__, const Request& request __attribute_maybe_unused__)
{
	return (NULL);
}