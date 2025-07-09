/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Download.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:59:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/09 15:32:26 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <sys/stat.h>

Response* handle_download_request(ServerConfig conf, const Request& request)
{
	std::string path = conf.getRoot() + ft_traductor(request.getUri());
	Response* response = NULL;

	if (access(path.c_str(), F_OK) != 0)
	{
		response = new Response(404, "Not Found");
		std::string error_body = get_custom_error_page(conf, 404);
		response->setBody(error_body);
		return response;
	}
	if (access(path.c_str(), R_OK) != 0)
	{
		response = new Response(403, "Forbidden");
		std::string error_body = get_custom_error_page(conf, 403);
		response->setBody(error_body);
		return response;
	}
	try
	{
		std::string body = get_file_content(path);
		response = new Response(200, "OK");
		response->setBody(body);

		std::string filename = ft_traductor(request.getUri());
		size_t last_slash = filename.find_last_of('/');
		if (last_slash != std::string::npos)
			filename = filename.substr(last_slash + 1);
		response->addHeader("Content-Disposition", "attachment; filename=\"" + filename + "\"");
		response->setRessourcePath(ft_traductor(request.getUri()));
		response->defineContentType();
		std::ostringstream content_length;
		content_length << body.length();
		response->addHeader("Content-Length", content_length.str());
		return response;
	}
	catch(const Response::ResourceNotFoundException& e) {
		response = new Response(404, "Not Found");
		std::string error_body = get_custom_error_page(conf, 404);
		response->setBody(error_body);
		return response;
	}
	catch(const Response::InternalServerErrorException& e) {
		response = new Response(500, "Internal Server Error");
		std::string error_body = get_custom_error_page(conf, 500);
		response->setBody(error_body);
		return response;
	}
}
