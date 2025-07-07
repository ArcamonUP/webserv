/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InitServers.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 10:35:08 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/04 16:53:24 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <cstring>

void signal_handler(int sig)
{
    if (sig == SIGINT || sig == SIGTERM) {
        g_signal = true;
        std::cout << std::endl << "Arrêt des serveurs demandé..." << std::endl;
    }
}

int create_server_socket(int sockfd, ServerConfig& server_config)
{
    int opt = 1;
    sockaddr_in sockaddr;

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        return (std::perror("setsockopt SO_REUSEADDR"), -1);
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        return (std::perror("setsockopt SO_REUSEPORT"), -1);
    if (make_not_blocking_socket(sockfd) == -1)
        return (std::perror("fcntl"), -1);
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(server_config.getPort());
    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cerr << "Failed to bind on port " << server_config.getPort() 
                  << ": " << strerror(errno) << std::endl;
        return (-1);
    }

    if (listen(sockfd, SOMAXCONN) < 0)
        return (std::perror("listen"), -1);
    server_config.setSockfd(sockfd);
    server_config.setSockaddr(sockaddr);
    std::cout << "Server listening on port " << server_config.getPort() << std::endl;
    return (0);
}

Config init(int ac, char **av)
{
    Config conf;
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    if (ac != 2 || !endsWith(av[1], ".conf"))
        throw (std::invalid_argument("Respect the format: ./WebServ [file].conf"));
    conf = Config(av[1]);
    return (conf);
}
