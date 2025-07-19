/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleLocationBlock.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:05:33 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/18 17:34:14 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

bool	handle_authorized_methods(ServerConfig& conf, Request *request, Connection* connection, \
	int l_index, int client_fd)
{
	std::vector<std::string>	allow = conf.getLocations()[l_index].getAllowedMethods();
	std::string					used_method = request->getMethod();

	if (allow.empty())
		return (false);
	for (size_t i = 0; i < allow.size(); ++i)
	{
		if (allow[i] == used_method)
			return (false);
	}
	Response response = Response(405, "Method Not Allowed");
	std::string error_body = get_custom_error_page(conf, 405);
	response.setBody(error_body);
	std::string serialized_response = response.getSerializedResponse();
	send(client_fd, serialized_response.c_str(), serialized_response.size(), NO_FLAGS);
	connection->reset();
	return (true);
}

bool	handle_max_size_body(ServerConfig& conf, Request *request, Connection* connection, \
	int l_index, int client_fd)
{
	size_t max_size = conf.getLocations()[l_index].getMaxSizeBody();

	if (max_size >= request->getBody().length())
		return (false);
	
	Response response = Response(413, "Payload Too Large");
	std::string error_body = get_custom_error_page(conf, 413);
	response.setBody(error_body);
	std::string serialized_response = response.getSerializedResponse();
	send(client_fd, serialized_response.c_str(), serialized_response.size(), NO_FLAGS);
	connection->reset();
	return (true);
}

bool	handle_location_redirect(ServerConfig& conf, Connection* connection, int l_index, int client_fd)
{
	int	redirectCode = conf.getLocations()[l_index].getRedirectCode();

	if (redirectCode == 0)
		return (false);
	
	std::string redirectUrl = conf.getLocations()[l_index].getRedirect();
	if (redirectUrl.empty())
		return (false);
	
	Response response = Response(redirectCode, "Moved");
	response.setLocation(redirectUrl);
	std::string body = "<!DOCTYPE html>\n<html><head><title>Redirecting...</title></head><body>\n";
	body += "<h1>Redirecting</h1>\n";
	body += "<p>The document has moved <a href=\"" + redirectUrl + "\">here</a>.</p>\n";
	body += "</body></html>";
	response.setBody(body);
	std::string serialized_response = response.getSerializedResponse();
	send(client_fd, serialized_response.c_str(), serialized_response.size(), NO_FLAGS);
	connection->reset();
	return (true);
}

int	handle_location_block(ServerConfig& conf, Connection* connection, Request *request, int client_fd)
{
	int	l_index;

	l_index = find_matching_location_index(conf, request->getUri());
	if (l_index == -1)
		return (-1);
	if (handle_authorized_methods(conf, request, connection, l_index, client_fd))
		return (CLOSE_CONNECTION);
	if (handle_max_size_body(conf, request, connection, l_index, client_fd))
		return (CLOSE_CONNECTION);
	if (handle_location_redirect(conf, connection, l_index, client_fd))
		return (CLOSE_CONNECTION);
	return (-1);
}
