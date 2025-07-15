/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 11:05:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/15 12:23:25 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <sys/stat.h>

std::map<std::string, MethodHandler> method_map;

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

int	handle_redir(ServerConfig& conf, Connection* connection, Request request, int client_fd)
{
	Response	*response;
	std::string	serialized_response;

	if (request.getUri().length() > 1 && request.getUri()[request.getUri().length() - 1] == '/')
	{
		std::string redirect_uri = request.getUri().substr(0, request.getUri().length() - 1);
		
		struct stat file_stat;
		std::string file_path = conf.getRoot() + redirect_uri.substr(1);
		std::cout << file_path << std::endl;
		if (stat(file_path.c_str(), &file_stat) == 0 && S_ISREG(file_stat.st_mode))
		{
			response = new Response(301, "MOVED PERMANENTLY");
			response->setLocation(redirect_uri);
			serialized_response = response->getSerializedResponse();
			send(client_fd, serialized_response.c_str(), serialized_response.size(), NO_FLAGS);
			connection->reset();
			std::cout << "\033[36mRedirection 301 sent to client " << client_fd - 5 << " from " << request.getUri() << " to " << redirect_uri << ".\033[0m" << std::endl;
			std::cout << "\033[33mConnection with client " << client_fd - 5 << " has been closed.\033[0m" << std::endl;
			return (delete (response), true);
		}
	}
	return (false);
}

int	handle_request(epoll_event *events, ServerConfig& conf)
{
	int client_fd = events->data.fd;
	int behaviour = connection_handler.handle_client_data(client_fd);
	if (behaviour == ERROR || behaviour == REQUEST_INCOMPLETE)
		return (behaviour);
	Connection * connection = connection_handler.get_connection(client_fd);
	if (!connection)
		return (ERROR);
	Request	request(connection->get_request());
	if (request.getError())
	{
		connection->reset();
		return (ERROR);
	}
	
	if (handle_redir(conf, connection, request, client_fd))
		return (CLOSE_CONNECTION);
	else if (is_cgi(conf, request))
	{
		connection->reset();
		if (cgi(request, client_fd, conf) != 0)
			return (ERROR);
		std::cout << "\033[34mA CGI response has been sent to client " << client_fd - 5 << ".\033[0m" << std::endl;
		return (SUCCESS);
	}
	else 
	{
		Response *response = handle_action(conf, request);
		std::string serialized_response = response->getSerializedResponse();
		send(client_fd, serialized_response.c_str(), serialized_response.size(), NO_FLAGS);
		connection->reset();
		std::cout << "\033[35mA response has been sent to client " << client_fd - 5 << ".\033[0m" << std::endl;
		if (response->getHeaderValue("connection") == "keep-alive")
			return (delete (response), KEEP_ALIVE);
		std::cout << "\033[33mConnection with client " << client_fd - 5 << " has been closed.\033[0m" << std::endl;
		return (delete (response), CLOSE_CONNECTION);
	}
}
