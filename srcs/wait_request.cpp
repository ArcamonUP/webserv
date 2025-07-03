/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:39:47 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/03 11:22:05 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

std::map<std::string, MethodHandler> method_map;

//Test function -> must change
std::string get_file_body(const Request& request)
{
	char buffer[4096];
	std::string file_path = "." + request.getUri();
	std::cout << file_path << std::endl;
	int fd = open(file_path.c_str(), O_RDONLY);
	if (errno == ENOENT)
		throw Response::ResourceNotFoundException();
	else if (fd < 0)
	{
		std::cout << errno << std::endl;
		throw Response::InternalServerErrorException();
	}
	int read_ret = read(fd, buffer, sizeof(buffer));
	if (read_ret < 0)
	{
		std::cout << errno << std::endl;
		throw Response::InternalServerErrorException();
	}
	std::string result(buffer, read_ret);
	return (result);
}
//buffer isn't dynamic
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

int accept_new(int server_fd, sockaddr_in sockaddr, epoll_event &ev, int epoll_fd)
{
	socklen_t	addrlen;
	int			client_fd;
	
	addrlen = sizeof(sockaddr);
	client_fd = accept(server_fd, (struct sockaddr*)&sockaddr, &addrlen);
	if (client_fd == -1)
		return (std::perror("accept"), 1);
	if (make_not_blocking_socket(client_fd) == -1)
		return (std::perror("make_not_blocking_socket"), close(client_fd), 1);
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = client_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1)
		return (std::perror("epoll_ctl"), close(client_fd), 1);
	return (0);
}

Response*	handle_action(const Request& request)
{
	Response *response;
	std::map<std::string, MethodHandler>::const_iterator it;
	std::string method_request;
	 
	method_request = request.getMethod();
	it = method_map.find(method_request);
	if (it != method_map.end())
	{
		response = (*method_map[method_request])(request);
		return (response);
	}
	else
	{
		response = new Response(501, "Not Implemented");
		return (response);
	}
}

void	handle_request(epoll_event *events)
{
	Response*	response;
	std::string serialized_response;
	int client_fd = events->data.fd;

	std::string serialized_request = get_request(client_fd);
	std::cout << "serialized request : \n" << serialized_request << std::endl;
	
	Request	request(serialized_request);
	std::cout << "parsed request : \n" << request << std::endl;
	
	response = handle_action(request);
	serialized_response = response->getSerializedResponse();
	
	send(client_fd, serialized_response.c_str(), serialized_response.size(), NO_FLAGS);
	close(client_fd);
	
	delete (response);
}

int	wait_request(int fd, sockaddr_in sockaddr)
{
	int	epoll_fd, nbfds;
	epoll_event ev, events[MAX_EVENTS];

	epoll_fd = epoll_create(1);
	if (epoll_fd == -1)
		return (std::perror("epoll_create"), 1);
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1)
		return (std::perror("epoll_ctl"), 1);
	while (true)
	{
		nbfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); //Max events: se baser sur NGINX
		if (nbfds == -1) {
			std::perror("epoll_wait");
			break ;
		}
		for (int i = 0; i < nbfds; i++)
		{
			if (events[i].data.fd == fd)
				accept_new(fd, sockaddr, ev, epoll_fd);
			else
				handle_request(events);
		}
	}
	(close(fd), close(epoll_fd));
	return (0);
}
