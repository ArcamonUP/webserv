/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:39:47 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/25 15:00:43 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <cstdio>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include "Webserv.hpp"
#include "Request.hpp"

# define MAX_EVENTS 32
# define NO_FLAGS 0

static std::string get_file_body(const Request& request)
{
	char buffer[4096];
	std::string file_path = "." + request.getUri();
	std::cout << file_path << std::endl;
	int fd = open(file_path.c_str(), O_RDONLY);
	if (errno == ENOENT)
		throw Request::ResourceNotFoundException();
	else if (fd < 0)
		std::cout << errno << std::cout, std::exit(EXIT_FAILURE);
	int read_ret = read(fd, buffer, sizeof(buffer));
	if (read_ret < 0)
		std::exit(EXIT_FAILURE);
	std::string result(buffer, read_ret);
	return (result);
}

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

void answer(epoll_event *events)
{
	int client_fd = events->data.fd;

	std::string request = get_request(client_fd);
	std::cout << "raw request : \n" << request << std::endl;
	Request curr_req(request);
	std::cout << "parsed request : \n" << curr_req << std::endl;
	std::string response;
	if (curr_req.getMethod() == "GET" && !curr_req.getUri().empty())
	{
		std::string file_body;
		try
		{
			file_body = get_file_body(curr_req);
			response = 
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: ";
			std::ostringstream oss;
			oss << file_body.length();
			response += oss.str();
			response += "\r\n";
			response += "Connection: close\r\n";
			response += "\r\n";
			response += file_body;
			response += "\n";
		}
		catch(const std::exception& e)
		{
			response =
			"HTTP/1.1 404 Not Found\r\n"
			"Connection: close\r\n"
			"\r\n"
			"Resource not found !\n";
		}
	}
	else if (curr_req.getMethod() == "POST")
	{
		std::string data = curr_req.getBody();
		std::cout << data << std::endl;
		response =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: ";
		std::ostringstream oss;
		oss << data.length();
		response += oss.str();
		response += "\r\n";
		response += "Connection: close\r\n";
		response += "\r\n";
		response += data;
		response += "\n";
	}
	send(client_fd, response.c_str(), response.size(), NO_FLAGS);
	close(client_fd);
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
				answer(events);
		}
	}
	(close(fd), close(epoll_fd));
	return (0);
}
