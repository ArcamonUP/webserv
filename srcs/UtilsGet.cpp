/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilsGet.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:30:00 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/08 15:41:23 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <sys/stat.h>

Response* handle_stopserv_request(ServerConfig& conf)
{
	std::string stop_page_path = conf.getRoot();
	if (!stop_page_path.empty() && stop_page_path[stop_page_path.length() - 1] != '/') {
		stop_page_path += "/";
	}
	stop_page_path += conf.getStopServer();
	std::string body = get_file_content(stop_page_path);
	Response* response = new Response(200, "OK");
	response->setBody(body);
	return response;
}

std::string build_file_path(ServerConfig& conf, const std::string& uri)
{
	if (uri == "/")
		return conf.getRoot() + "/" + conf.getIndex();
	else
		return conf.getRoot() + uri;
}

Response* handle_directory_request(ServerConfig& conf, const std::string& file_path, const std::string& uri, int location_index)
{
	std::string index_file = file_path;
	if (file_path.empty() || file_path[file_path.length() - 1] != '/') 
		index_file += "/";
	
	std::string index_name = "index.html";
	if (location_index >= 0 && !conf.getLocations()[location_index].getIndex().empty()) {
		index_name = conf.getLocations()[location_index].getIndex();
	}
	index_file += index_name;
	
	if (access(index_file.c_str(), F_OK) == 0) {
		std::string body = get_file_content(index_file);
		Response* response = new Response(200, "OK");
		response->setBody(body);
		return response;
	} else {
		bool autoindex_enabled = false;
		if (location_index >= 0) {
			autoindex_enabled = conf.getLocations()[location_index].getAutoIndex();
		}
		
		if (autoindex_enabled) {
			std::string body = generate_autoindex(file_path, uri);
			Response* response = new Response(200, "OK");
			response->setBody(body);
			return response;
		} else {
			Response* response = new Response(403, "Forbidden");
			std::string error_body = get_custom_error_page(conf, 403);
			response->setBody(error_body);
			return response;
		}
	}
}

Response* handle_file_request(const std::string& file_path)
{
	std::string body = get_file_content(file_path);
	Response* response = new Response(200, "OK");
	response->setBody(body);
	return response;
}
