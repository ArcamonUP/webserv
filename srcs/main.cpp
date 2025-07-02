/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:38 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/02 17:55:05 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include "Webserv.hpp"
#include "Config.hpp"

void	end(pid_t *tab_pid, size_t n)
{
	size_t	i = 0;

	while (i < n)
		waitpid(tab_pid[i++], NULL, 0);
	delete[] tab_pid;
	return ;
}

int	start_serv(Config conf, size_t n)
{
	sockaddr_in	sockaddr;
	int			sockfd;
	int			opt = 1;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return (std::perror("socket"), 1);
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		return (std::perror("setsockopt"), 1);
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
		return (std::perror("setsockopt SO_REUSEPORT"), 1);
	if (make_not_blocking_socket(sockfd) == -1)
		return (std::perror("fcntl"), 1);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(conf.getServer()[n].getPort());
	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
		std::cerr << "Failed to bind on port " << conf.getServer()[n].getPort() 
				  << " (Server " << n << "): " << strerror(errno) << std::endl;
		return (close(sockfd), 1);
	}
	if (listen(sockfd, SOMAXCONN) < 0)
		return (std::perror("listen"), 1);
	std::cout << "Server " << n << " listening on port " << conf.getServer()[n].getPort() << std::endl;
	return (wait_request(sockfd, sockaddr, conf.getServer()[n]));
}

int	main(int ac, char **av)
{
	Config conf;
	pid_t		*tab_pid = NULL;
	size_t		nb_servers = 0;

	if (ac != 2 || !endsWith(av[1], ".conf")) {
		std::cerr << "Respect the format: ./webserv [file].conf" << std::endl;
		return (1);
	}
	try {conf = Config(av[1]);}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	nb_servers = conf.getServer().size();
	tab_pid = new pid_t[nb_servers];
	if (!tab_pid)
		return (std::perror("new"), 1);
	for (size_t n = 0; n < nb_servers; n++) {
		pid_t pid = fork();
		if (pid == -1) {
			std::perror("fork");
			for (size_t i = 0; i < n; i++)
				kill(tab_pid[i], SIGTERM);
			end(tab_pid, n);
			delete[] tab_pid;
			return (1);
		}
		if (pid == 0) {
			delete[] tab_pid;
			return (start_serv(conf, n));
		}
		tab_pid[n] = pid;
	}
	end(tab_pid, nb_servers);
	return (0);
}
