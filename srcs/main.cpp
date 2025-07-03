/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:38 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/03 15:33:16 by kbaridon         ###   ########.fr       */
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
#include <vector>
#include <map>
#include "Webserv.hpp"
#include "Config.hpp"

volatile sig_atomic_t g_shutdown_requested = 0;

void signal_handler(int sig)
{
    if (sig == SIGINT || sig == SIGTERM) {
        g_shutdown_requested = 1;
        std::cout << "\nArrêt des serveurs demandé..." << std::endl;
    }
}

int create_server_socket(ServerConfig& server_config)
{
    int sockfd;
    int opt = 1;
    sockaddr_in sockaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::perror("socket");
        return -1;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::perror("setsockopt SO_REUSEADDR");
        close(sockfd);
        return -1;
    }
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        std::perror("setsockopt SO_REUSEPORT");
        close(sockfd);
        return -1;
    }

    if (make_not_blocking_socket(sockfd) == -1) {
        std::perror("fcntl");
        close(sockfd);
        return -1;
    }

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(server_config.getPort());

    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cerr << "Failed to bind on port " << server_config.getPort() 
                  << ": " << strerror(errno) << std::endl;
        close(sockfd);
        return -1;
    }

    if (listen(sockfd, SOMAXCONN) < 0) {
        std::perror("listen");
        close(sockfd);
        return -1;
    }

    server_config.setSockfd(sockfd);
    server_config.setSockaddr(sockaddr);

    std::cout << "Server listening on port " << server_config.getPort() << std::endl;
    return sockfd;
}

int wait_all_servers(std::vector<ServerConfig>& servers)
{
    if (servers.empty()) {
        std::cerr << "Aucun serveur configuré" << std::endl;
        return 1;
    }
    
    return wait_multiple_servers(servers);
}

int	main(int ac, char **av)
{
    Config conf;

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    if (ac != 2 || !endsWith(av[1], ".conf")) {
        std::cerr << "Respect the format: ./webserv [file].conf" << std::endl;
        return (1);
    }
    
    try {
        conf = Config(av[1]);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }

    std::vector<ServerConfig> servers = conf.getServer();
    
    for (size_t i = 0; i < servers.size(); i++) {
        if (create_server_socket(servers[i]) == -1) {
            std::cerr << "Échec de l'initialisation du serveur " << i 
                      << " sur le port " << servers[i].getPort() << std::endl;
            
            for (size_t j = 0; j < i; j++) {
                if (servers[j].getSockfd() != -1) {
                    close(servers[j].getSockfd());
                }
            }
            return 1;
        }
    }

    std::cout << "Tous les serveurs sont démarrés. En attente de connexions..." << std::endl;
    
    int result = wait_all_servers(servers);
    
    for (size_t i = 0; i < servers.size(); i++) {
        if (servers[i].getSockfd() != -1) {
            close(servers[i].getSockfd());
        }
    }
    
    return result;
}
