/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:39:47 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/24 18:02:04 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <cstdio>
#include "Webserv.hpp"
#include "Request.hpp"
#include <fstream>
#include <sstream> 

# define MAX_EVENTS 32
# define NO_FLAGS 0

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
	std::cout << "> Request from client:\n" << Request(request);
	
	std::ifstream file("srcs/www/index.html");
	if (!file.is_open())
	{
		std::string error_response =
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 21\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Erreur serveur interne";
        send(client_fd, error_response.c_str(), error_response.size(), 0);
        close(client_fd);
        return;
	}
	std::istreambuf_iterator<char> begin(file);
	std::istreambuf_iterator<char> end;
	std::string html_body(begin, end);
					
	file.close();

	std::ostringstream oss;
	oss << html_body.size();

	std::string response =
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html\r\n"
	"Content-Length: " + oss.str() + "\r\n"
	"connecton: close\r\n"
	"\r\n" + 
	html_body;

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
