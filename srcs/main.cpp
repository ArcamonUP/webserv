/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:38 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/03 02:58:55 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

int	main()
{
	sockaddr_in	sockaddr;
	int			sockfd;
	int			opt = 1;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return (std::perror("socket"), 1);
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		return (std::perror("setsockopt"), 1);
	if (make_not_blocking_socket(sockfd) == -1)
		return (std::perror("fcntl"), 1);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(PORTS);
	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
		return (std::perror("bind"), 1);
	if (listen(sockfd, SOMAXCONN) < 0)
		return (std::perror("listen"), 1);
	initMethodMap();
	return (wait_request(sockfd, sockaddr));
}
