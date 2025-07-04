/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WaitRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:39:47 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/04 16:53:24 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

# define MAX_EVENTS 512

int	init_epoll(int *epoll_fd, epoll_event *ev, std::vector<ServerConfig>* servers, \
	std::map<int, ServerConfig*> *server_map)
{
	(*epoll_fd) = epoll_create(1);
	if (*epoll_fd == -1)
		return (std::perror("epoll_create"), 1);

	for (size_t i = 0; i < servers->size(); i++)
	{
		ev->events = EPOLLIN;
		ev->data.fd = (*servers)[i].getSockfd();
		if (epoll_ctl(*epoll_fd, EPOLL_CTL_ADD, (*servers)[i].getSockfd(), ev) == -1)
		{
			close(*epoll_fd);
			return (std::perror("epoll_ctl"), 1);
		}
		(*server_map)[(*servers)[i].getSockfd()] = &((*servers)[i]);
	}
	return (0);
}

void accept_new(int fd, ServerConfig* serv, \
	std::map<int, ServerConfig*> *client_map, epoll_event *ev, int epoll_fd)
{
	sockaddr_in	serv_addr = serv->getSockaddr();
	socklen_t	addrlen = sizeof(serv_addr);
	int			client_fd = accept(fd, (struct sockaddr*)&serv_addr, &addrlen);

	if (client_fd == -1)
		return (std::perror("accept"), (void)0);
	if (make_not_blocking_socket(client_fd) == -1) {
		close(client_fd);
		return (std::perror("make_not_blocking_socket"), (void)0);
	}
	ev->events = EPOLLIN | EPOLLET;
	ev->data.fd = client_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, ev) == -1) {
		close(client_fd);
		return (std::perror("epoll_ctl"), (void)0);
	}
	(*client_map)[client_fd] = serv;
	return ;
}

int	pre_answer(int fd, int epoll_fd, int i, epoll_event *events, ServerConfig* serv, \
	std::map<int, ServerConfig*> *serv_map, std::map<int, ServerConfig*> *client_map)
{
	int result = handle_request(&events[i], *serv);

	if (result == 1)
		(*client_map).erase(fd);
	if (result == 2)
	{
		(*client_map).erase(fd);
		if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, serv->getSockfd(), NULL) == -1)
			std::perror("epoll_ctl DEL");
		close(serv->getSockfd());
		(*serv_map).erase(serv->getSockfd());
		if ((*serv_map).empty())
		{
			std::cout << "Tous les serveurs ont ete stoppes. Arret du programme." << std::endl;
			close(epoll_fd);
			return (1);
		}
	}
	return (0);
}

int wait_multiple_servers(std::vector<ServerConfig>& servers)
{
	int epoll_fd, nbfds, curr_fd;
	epoll_event ev, events[MAX_EVENTS];
	std::map<int, ServerConfig*> server_map;
	std::map<int, ServerConfig*> client_map;

	if (init_epoll(&epoll_fd, &ev, &servers, &server_map))
		return (1);
	while (true)
	{
		nbfds = epoll_wait(epoll_fd, events, MAX_EVENTS, 1000);
		if (g_signal)
			return (close(epoll_fd), 0);
		if (nbfds == -1) {
			std::perror("epoll_wait");
			break ;
		}
		else if (nbfds == 0)
			continue;
		for (int i = 0; i < nbfds; i++) {
			curr_fd = events[i].data.fd;
			if (server_map.find(curr_fd) != server_map.end())
				accept_new(curr_fd, server_map[curr_fd], &client_map, &ev, epoll_fd);
			else if (pre_answer(curr_fd, epoll_fd, i, events, client_map[curr_fd], \
				&server_map, &client_map) == 1)
				return (0);
		}
	}
	return (close(epoll_fd), 1);
}
