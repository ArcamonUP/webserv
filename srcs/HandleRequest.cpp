/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 11:05:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/14 14:17:08 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

std::map<std::string, MethodHandler> method_map;

// std::string get_request(int connection)
// {
// 	char buffer[4096];
// 	std::string result;

// 	while (true)
// 	{
// 		ssize_t bytesRead = recv(connection, buffer, sizeof(buffer), 0);
// 		if (bytesRead <= 0)
// 			break;
// 		result.append(buffer, bytesRead);
// 		if (result.find("\r\n\r\n") != std::string::npos)
// 			break;
// 	}
// 	return result;
// }

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
		response->setBody(get_custom_error_page(conf, 501));
		return (response);
	}
}

int	handle_request(epoll_event *events, ServerConfig& conf)
{
	int client_fd = events->data.fd;
	int behaviour = connection_handler.handle_client_data(client_fd);
	std::cout << "handle client data res = " << behaviour << std::endl;
	if (behaviour == ERROR || behaviour == REQUEST_INCOMPLETE)
		return (behaviour);
	Connection * connection = connection_handler.get_connection(client_fd);
	if (!connection)
		return (ERROR);
	std::cout << "connection with " << client_fd << " is ok" << std::endl;
	std::string serialized_request = connection->get_request();
	std::cout << serialized_request << std::endl;
	Request	request(serialized_request);
	if (request.getError())
	{
		connection->reset();
		return (ERROR);
	}
	std::cout << "fd [" << client_fd << "] = " << serialized_request << std::endl;
	Response*	response;
	std::string serialized_response;
	if (is_cgi(conf, request))
	{
		if (cgi(request, client_fd, conf) == 0)
		{
			connection->reset();
			return (SUCCESS);
		}
	}
	else 
	{
		response = handle_action(conf, request);
		serialized_response = response->getSerializedResponse();
		std::cout << client_fd << std::endl;
		send(client_fd, serialized_response.c_str(), serialized_response.size(), NO_FLAGS);
		if (response->getHeaderValue("connection") == "keep-alive")
		{
			connection->reset();
			return (delete (response), KEEP_ALIVE);
		}
		delete (response);
		connection->reset();
		return (CLOSE_CONNECTION);
	}
	return (SUCCESS);
}
