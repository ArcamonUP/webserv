/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:47:11 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/18 11:36:37 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

Response*	handle_error_buttons(ServerConfig conf, std::string uri)
{
	Response *response = NULL;
	std::string error_codes[] = {"301", "400", "403", "404", "405", "411", "414", \
	"418", "429", "500", "501", "502", "503", "504", "505", ""};
	std::string error_messages[] = {"Moved Permanently", "Bad Request", "Forbidden", \
	"Not Found", "Method Not Allowed", "Lenght Required", "URI Too Long", "I'm a teapot", \
	"Too Many Requests", "Internal Server Error", "Not Implemented", "Bad Gateway", \
	"Service Unavailable", "Gateway Timeout", "HTTP Version Not Supported", ""};
	std::string basicinput = "/errorxxx";
	uri = uri.substr(basicinput.length() - 3);

	for (size_t i = 0; !error_codes[i].empty(); i++)
	{
		if (uri == error_codes[i])
		{
			response = new Response(ft_atoi(error_codes[i]), error_messages[i]);
			response->setBody(get_custom_error_page(conf, ft_atoi(error_codes[i])));
			return (response);
		}
	}
	return (response);
}

Response* handle_all_exceptions(ServerConfig conf)
{
	Response*	response = NULL;
	std::string	error_body;
	
	try {
		throw;
	}
	catch (const Response::ResourceForbiddenException&) {
		response = new Response(403, "Forbidden");
		error_body = get_custom_error_page(conf, 403);
		response->setBody(error_body);
		return (response);
	}
	catch (const Response::ResourceNotFoundException&) {
		response = new Response(404, "Not Found");
		error_body = get_custom_error_page(conf, 404);
		response->setBody(error_body);
		return (response);
	}
	catch (const Response::InternalServerErrorException&) {
		response = new Response(500, "Internal Servor Error");
		error_body = get_custom_error_page(conf, 500);
		response->setBody(error_body);
		return (response);
	}
}

bool	is_button_error(const Request &request)
{
	std::string	uri = request.getUri();
	std::string	basicinput = "/errorxxx";
	std::string possibles[] = {"301", "400", "403", "404", "405", "411", "414", \
	"418", "429", "500", "501", "502", "503", "504", "505", ""};

	if (uri.length() != basicinput.length() || uri.find("/error") != 0)
		return (false);
	uri = uri.substr(basicinput.length() - 3);
	for (size_t i = 0; !possibles[i].empty(); i++)
	{
		if (uri == possibles[i])
			return (true);
	}
	return (false);
}

std::string get_default_error_page(int error_code)
{
	std::ostringstream html;
	html << "<!DOCTYPE html><html><head><title>Error " << error_code << "</title>";
	html << "<style>body{font-family:'Segoe UI',sans-serif;margin:0;background:linear-gradient(135deg,#667eea,#764ba2);min-height:100vh;display:flex;align-items:center;justify-content:center}";
	html << ".container{background:white;padding:40px;border-radius:15px;box-shadow:0 20px 40px rgba(0,0,0,0.1);text-align:center;max-width:500px}";
	html << "h1{color:#e74c3c;margin-bottom:20px}p{color:#666;margin-bottom:30px}";
	html << ".btn{background:linear-gradient(45deg,#667eea,#764ba2);color:white;padding:12px 24px;border-radius:25px;text-decoration:none;display:inline-block}</style></head>";
	html << "<body><div class=\"container\"><h1>Error " << error_code << "</h1>";
	
	switch (error_code) {
		case 301: html << "<p>Moved Permanently.</p>"; break;
		case 400: html << "<p>Invalid request.</p>"; break;
		case 403: html << "<p>Resource access is forbidden.</p>"; break;
		case 404: html << "<p>Resource not found.</p>"; break;
		case 405: html << "<p>Unauthorized method</p>"; break;
		case 411: html << "<p>Lenght Required.</p>"; break;
		case 414: html << "<p>URI Too Long.</p>"; break;
		case 418: html << "<p>I'm a teapot.</p>"; break;
		case 429: html << "<p>Too Many Requests</p>"; break;
		case 500: html << "<p>Server encountered an intern error.</p>"; break;
		case 501: html << "<p>Not Implemented.</p>"; break;
		case 502: html << "<p>Bad Gateway.</p>"; break;
		case 503: html << "<p>Service is unavailable.</p>"; break;
		case 504: html << "<p>Timeout gateway.</p>"; break;
		case 505: html << "<p>HTTP Version Not Supported.</p>"; break;
		default: html << "<p>An unknown error occured.</p>"; break;
	}
	return html.str();
}

std::string get_custom_error_page(ServerConfig& conf, int error_code)
{
	const std::map<int, std::string>& error_pages = conf.getErrorPages();
	std::map<int, std::string>::const_iterator it = error_pages.find(error_code);
	
	if (it != error_pages.end()) {
		std::string error_page_path = conf.getRoot() + it->second;
		try {
			return get_file_content(error_page_path);
		} catch (const Response::ResourceNotFoundException&) {
			std::cerr << "Error: Custom error page not found: " << error_page_path << std::endl;
		} catch (const Response::InternalServerErrorException&) {
			std::cerr << "Error: Cannot read custom error page: " << error_page_path << std::endl;
		} catch (const std::exception& e) {
			std::cerr << "Error reading custom error page: " << e.what() << std::endl;
		}
	}
	return get_default_error_page(error_code);
}
