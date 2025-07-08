/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Download.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:59:36 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/08 18:50:25 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <sys/stat.h>

std::string get_mime_type(const std::string& filename)
{
	size_t dot_pos = filename.find_last_of('.');
	if (dot_pos == std::string::npos)
		return "application/octet-stream";

	//Temporaire, on va utiliser la fonction de mateo
	std::string ext = filename.substr(dot_pos + 1);
	if (ext == "txt") return "text/plain";
	if (ext == "html" || ext == "htm") return "text/html";
	if (ext == "css") return "text/css";
	if (ext == "js") return "application/javascript";
	if (ext == "json") return "application/json";
	if (ext == "pdf") return "application/pdf";
	if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
	if (ext == "png") return "image/png";
	if (ext == "gif") return "image/gif";
	if (ext == "zip") return "application/zip";
	if (ext == "tar") return "application/x-tar";
	if (ext == "gz") return "application/gzip";
	if (ext == "mp3") return "audio/mpeg";
	if (ext == "mp4") return "video/mp4";
	if (ext == "avi") return "video/x-msvideo";
	if (ext == "doc") return "application/msword";
	if (ext == "docx") return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	if (ext == "xls") return "application/vnd.ms-excel";
	if (ext == "xlsx") return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	return "application/octet-stream";
}

Response* handle_download_request(ServerConfig conf, const Request& request)
{
	std::string path = conf.getRoot() + request.getUri();
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

		std::string filename = request.getUri();
		size_t last_slash = filename.find_last_of('/');
		if (last_slash != std::string::npos)
			filename = filename.substr(last_slash + 1);

		response->addHeader("Content-Disposition", "attachment; filename=\"" + filename + "\"");

		std::string content_type = get_mime_type(filename);
		response->addHeader("Content-Type", content_type);

		std::ostringstream content_length;
		content_length << body.length();
		response->addHeader("Content-Length", content_length.str());

		return response;
	}
	catch(const Response::ResourceNotFoundException& e)
	{
		response = new Response(404, "Not Found");
		std::string error_body = get_custom_error_page(conf, 404);
		response->setBody(error_body);
		return response;
	}
	catch(const Response::InternalServerErrorException& e)
	{
		response = new Response(500, "Internal Server Error");
		std::string error_body = get_custom_error_page(conf, 500);
		response->setBody(error_body);
		return response;
	}
}
