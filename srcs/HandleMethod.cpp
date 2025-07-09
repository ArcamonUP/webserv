/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:33:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/09 16:37:50 by kbaridon         ###   ########.fr       */
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
	std::string body, file_path, uri, query_string = "";
	size_t		query_pos, location_index;
	struct stat	path_stat;
	Response	*response = NULL;

	try
	{
		uri = request.getUri();
		query_pos = uri.find('?');
		
		if (query_pos != std::string::npos) {
			query_string = uri.substr(query_pos + 1);
			uri = uri.substr(0, query_pos);
		}
		
		if (uri == "/stopserv")
			return (handle_stopserv_request(conf));
		if (query_string.find("download=1") != std::string::npos)
			return (handle_download_request(conf, uri));

		location_index = find_matching_location_index(conf, uri);
		file_path = build_file_path(conf, uri);
		if (stat(file_path.c_str(), &path_stat) == 0)
		{
			if (S_ISDIR(path_stat.st_mode))
				return (handle_directory_request(conf, file_path, uri, location_index));
			else
				return (handle_file_request(file_path));
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
		std::string path;
		Response *response = NULL;


		path = conf.getRoot() + ft_traductor(request.getUri());
		if (access(path.c_str(), F_OK) != 0)
		{
				response = new Response(404, "Not Found");
				response->setBody(get_custom_error_page(conf, 404));
				return response;

		}
		if (access(path.c_str(), W_OK ) != 0)
		{
			response = new Response(403, "Forbidden");
			response->setBody(get_custom_error_page(conf, 403));
			return response;
		}
		std::string::size_type pos = path.find_last_of('/');
		if (pos == std::string::npos)
		{
			response = new Response(500, "Internal Server Error");
			response->setBody(get_custom_error_page(conf, 500));
			return response;
		}
		std::string dir = path.substr(0, pos);
		if (access(dir.c_str(), W_OK | X_OK))
		{
			response = new Response(403, "Forbidden");
			response->setBody(get_custom_error_page(conf, 403));
			return response;
		}
		if(remove(path.c_str()) != 0)
		{
			response = new Response(403, "Forbidden");
			response->setBody(get_custom_error_page(conf, 403));
			return response;
		}
		return response = new Response(200, "OK");	
}