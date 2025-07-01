/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:38 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/01 13:24:47 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <iostream>
#include <cstdio>
#include "Webserv.hpp"
#include "Config.hpp"

# define PORTS 8080

int	main(int ac, char **av)
{
	Config conf;
	sockaddr_in	sockaddr;
	int			sockfd;
	int			opt = 1;

	if (ac != 2 || !endsWith(av[1], ".conf")) {
		std::cerr << "Respect the format: ./webserv [file].conf" << std::endl;
		return (1);
	}
	try {conf = Config(av[1]);}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	std::cout << conf << std::endl;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return (std::perror("socket"), 1);
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		return (std::perror("setsockopt"), 1);
	if (make_not_blocking_socket(sockfd) == -1)
		return (std::perror("fcntl"), 1);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(conf.getServer()[0].getPort());
	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
		return (std::perror("bind"), 1);
	if (listen(sockfd, SOMAXCONN) < 0)
		return (std::perror("listen"), 1);
	return (std::perror("Done"), 0);
}
