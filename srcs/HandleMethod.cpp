/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:33:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/19 10:36:36 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <sys/stat.h>

Response*	HandleHEAD(ServerConfig conf, const Request& request)
{
	std::string file_path;
	size_t		location_index;
	struct stat	path_stat;
	Response	*response = NULL;

	try
	{
		location_index = find_matching_location_index(conf, request.getUri());
		file_path = build_file_path(conf, request.getUri());
		if (!request.getQueryString().empty() && request.getQueryString().find("download=1") != std::string::npos)
			response = handle_download_request(conf, request.getUri());
		else if (is_button_error(request))
			response = handle_error_buttons(conf, request.getUri());
		else if (stat(file_path.c_str(), &path_stat) != 0)
			throw (Response::ResourceNotFoundException());
		else if (S_ISDIR(path_stat.st_mode))
			response = handle_directory_request(conf, file_path, request.getUri(), location_index);
		else
			response = handle_file_request(file_path);
		response->setBody("");
		return (response);
	}
	catch (...) {
		response = handle_all_exceptions(conf);
		response->setBody("");
	}
	return (response);
}

Response*	HandleGET(ServerConfig conf, const Request& request)
{
	std::string file_path;
	size_t		location_index;
	struct stat	path_stat;
	Response	*response = NULL;
	
	try
	{
		if (!request.getQueryString().empty() && request.getQueryString().find("download=1") != std::string::npos)
			return (handle_download_request(conf, request.getUri()));
		if (is_button_error(request))
			return (handle_error_buttons(conf, request.getUri()));

		location_index = find_matching_location_index(conf, request.getUri());
		file_path = build_file_path(conf, request.getUri());
		if (stat(file_path.c_str(), &path_stat) != 0)
			throw (Response::ResourceNotFoundException());
		if (S_ISDIR(path_stat.st_mode))
			return (handle_directory_request(conf, file_path, request.getUri(), location_index));
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
		path = build_file_path(conf, request.getUri());
		
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
			return (handle_file_request(path));
		}
		catch (...) {
			response = handle_all_exceptions(conf);
		}
		return (response);
}
