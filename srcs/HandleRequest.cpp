/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 11:05:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/08 14:42:26 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#define NO_FLAGS 0

std::map<std::string, MethodHandler> method_map;

std::string get_request(int connection)
{
	char buffer[4096];
	std::string result;

	while (true)
	{
		ssize_t bytesRead = recv(connection, buffer, sizeof(buffer), 0);
		if (bytesRead <= 0)
			break;
		result.append(buffer, bytesRead);
		if (bytesRead < (ssize_t)sizeof(buffer))
			break ;
	}
	return result;
}

Response*	handle_action(ServerConfig& conf, Request& request)
{
	Response *response;
	std::map<std::string, MethodHandler>::const_iterator it;
	std::string method_request;
	 
	method_request = request.getMethod();
	it = method_map.find(method_request);
	if (it != method_map.end())
	{
		response = (*method_map[method_request])(conf, request);
		return (response);
	}
	else
	{
		response = new Response(501, "Not Implemented");
		return (response);
	}
}

int	handle_request(epoll_event *events, ServerConfig& conf)
{
	Response*	response;
	std::string serialized_response;
	int client_fd = events->data.fd;
	std::string serialized_request = get_request(client_fd);
	Request	request(serialized_request);
	if (request.getError())
		return (1);
	std::cout << "parsed request : \n" << request << std::endl;

	if (is_cgi(conf, request))
	{
		if (cgi(request, client_fd, conf) == 0)
			return (0);
	}
	else {
		response = handle_action(conf, request);
		serialized_response = response->getSerializedResponse();
		send(client_fd, serialized_response.c_str(), serialized_response.size(), NO_FLAGS);
		close(client_fd), delete (response);
		if (request.getUri() == "/stopserv")
			return (2);
	}
	return (1);
}
