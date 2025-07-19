/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilsGet.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:30:00 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/19 11:00:28 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <sys/stat.h>

Response* handle_download_request(ServerConfig conf, std::string uri)
{
	uri.resize(uri.find('?'));
	std::string path = conf.getRoot() + ft_traductor(uri);
	Response* response = NULL;

	if (access(path.c_str(), F_OK) != 0)
		throw (Response::ResourceNotFoundException());
	if (access(path.c_str(), R_OK) != 0)
		throw(Response::InternalServerErrorException());
	response = handle_file_request(path);
	size_t last_slash = path.find_last_of('/');
	if (last_slash != std::string::npos)
		path = path.substr(last_slash + 1);
	response->addHeader("Content-Disposition", "attachment; filename=\"" + path + "\"");
	return response;
}

Response* handle_directory_request(ServerConfig& conf, const std::string& file_path, const std::string& uri, int location_index)
{
	std::string index_file = file_path, index_name = conf.getIndex();

	if (file_path.empty() || file_path[file_path.length() - 1] != '/') 
		index_file += "/";
	if (location_index >= 0 && !conf.getLocations()[location_index].getIndex().empty())
		index_name = conf.getLocations()[location_index].getIndex();
	index_file += index_name;

	if (access(index_file.c_str(), F_OK) == 0)
		return (handle_file_request(index_file));
	bool autoindex_enabled = conf.getLocations()[location_index].getAutoIndex();
	if (autoindex_enabled) {
		std::string body = generate_autoindex(file_path, uri);
		Response* response = new Response(200, "OK");
		response->setBody(body);
		response->setRessourcePath(file_path);
		response->setContentType("text/html");
		return response;
	} else
		throw (Response::ResourceForbiddenException());
}

Response* handle_file_request(const std::string& ressource_path)
{
	std::string body = get_file_content(ressource_path);
	Response* response = new Response(200, "OK");
	response->setRessourcePath(ressource_path);
	response->defineContentType();
	response->setBody(body);
	return response;
}

std::string build_file_path(ServerConfig& conf, const std::string& uri)
{
	if (uri == "/")
		return conf.getRoot() + conf.getIndex();
	else
		return conf.getRoot() + uri;
}
