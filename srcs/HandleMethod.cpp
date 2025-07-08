/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:33:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/08 15:41:52 by kbaridon         ###   ########.fr       */
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
		
		if (uri == "/stopserv")
			return handle_stopserv_request(conf);
		int location_index = find_matching_location_index(conf, uri);
		file_path = build_file_path(conf, uri);
		struct stat path_stat;
		if (stat(file_path.c_str(), &path_stat) == 0) {
			if (S_ISDIR(path_stat.st_mode))
				response = handle_directory_request(conf, file_path, uri, location_index);
			else
				response = handle_file_request(file_path);
		}
		else
			throw Response::ResourceNotFoundException();
	}
	catch(const Response::ResourceNotFoundException& e) {
		response = new Response(404, "Not Found");
		std::string error_body = get_custom_error_page(conf, 404);
		response->setBody(error_body);
	}
	catch(const Response::InternalServerErrorException& e) {
		response = new Response(500, "Internal Server Error");
		std::string error_body = get_custom_error_page(conf, 500);
		response->setBody(error_body);
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
		{
			std::string stop_page_path = conf.getRoot() + "/" + conf.getStopServer();
			body = get_file_content(stop_page_path);
		}
		else
			body = get_file_content(conf.getRoot() + request.getUri());
		response = new Response(200, "OK");
		response->setBody(body);
	}
	catch(const Response::ResourceNotFoundException& e) {
		response = new Response(404, "Not Found");
		std::string error_body = get_custom_error_page(conf, 404);
		response->setBody(error_body);
	}
	catch(const Response::InternalServerErrorException& e) {
		response = new Response(500, "Internal Server Error");
		std::string error_body = get_custom_error_page(conf, 500);
		response->setBody(error_body);
	}
	return (response);
}

Response*	HandleDELETE(ServerConfig conf __attribute_maybe_unused__, const Request& request __attribute_maybe_unused__)
{
	return (NULL);
}