/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:33:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/09 18:07:45 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <sys/stat.h>

Response*	HandleHEAD(ServerConfig conf __attribute_maybe_unused__, const Request& request __attribute_maybe_unused__)
{
	return (NULL);
}

Response* test_errors(ServerConfig conf, std::string &uri)
{
	if (uri.find("/error403") != std::string::npos)
	{
		std::string path;
		Response *response = NULL;


		path = "fichier_test_no_permission.txt";
		if (access(path.c_str(), W_OK ) != 0)
		{
			response = new Response(403, "Forbidden");
			response->setBody(get_custom_error_page(conf, 403));
			response->setBody(get_file_content("./srcs/www/403.html"));
			return response;
		}     
	}	
	return NULL;
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
		if (query_string.find("download=1") != std::string::npos)
		{
			Request download_request = request;
			download_request.setUri(uri);
			return handle_download_request(conf, download_request);
		}
		response = test_errors(conf, uri);
		if (response)
			return response;
		int location_index = find_matching_location_index(conf, uri);
		
		if (uri == "/stopserv")
			return (handle_stopserv_request(conf));
		if (!query_string.empty() && query_string.find("&download=1") != std::string::npos)
			return (handle_download_request(conf, uri));

		location_index = find_matching_location_index(conf, uri);
		file_path = build_file_path(conf, uri);
		if (stat(file_path.c_str(), &path_stat) != 0)
			throw (Response::ResourceNotFoundException());
		if (S_ISDIR(path_stat.st_mode))
			return (handle_directory_request(conf, file_path, uri, location_index));
		return (handle_file_request(file_path));
	}
	catch (...) {
		response = handle_all_exceptions(conf);
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
	catch (...) {
		response = handle_all_exceptions(conf);
	}
	return (response);
}

Response*	HandleDELETE(ServerConfig conf __attribute_maybe_unused__, const Request& request __attribute_maybe_unused__)
{
		std::string path, dir;
		std::size_t	pos;
		Response *response = NULL;


		path = conf.getRoot() + ft_traductor(request.getUri());
		try
		{
			if (access(path.c_str(), F_OK) != 0)
				throw (Response::ResourceNotFoundException());
			if (access(path.c_str(), W_OK) != 0)
				throw (Response::ResourceForbiddenException());
			pos = path.find_last_of('/');
			if (pos == std::string::npos)
				throw (Response::InternalServerErrorException());
			dir = path.substr(0, pos);
			if (access(dir.c_str(), W_OK | X_OK) || remove(path.c_str()) != 0)
				throw (Response::ResourceForbiddenException());
			response = new Response(200, "OK");
		}
		catch (...) {
			response = handle_all_exceptions(conf);
		}
		response->setError(false);
		return (response);
}
