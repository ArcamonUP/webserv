/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:38 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/04 18:17:47 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

bool g_signal = false;

int start_server(std::vector<ServerConfig>& servers)
{
    int result;

    if (servers.empty())
    {
        std::cerr << "Aucun serveur configure. Stopping the program..." << std::endl;
        return (1);
    }
    std::cout << "Tous les serveurs sont démarrés. En attente de connexions..." << std::endl;
    result = wait_multiple_servers(servers);
    for (size_t i = 0; i < servers.size(); i++)
    {
        if (servers[i].getSockfd() != -1)
            close(servers[i].getSockfd());
    }
    return (result);
}

int	main(int ac, char **av)
{
    Config conf;

    try {conf = init(ac, av);}
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    initMethodMap(), Response::initBuilders(), Response::initContentTypes();
    std::vector<ServerConfig> servers = conf.getServer();
    for (size_t i = 0; i < servers.size(); i++)
    {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1 || create_server_socket(sockfd, servers[i]) == -1)
        {
            if (sockfd != -1)
                close(sockfd);
            std::cerr << "Échec de l'initialisation du serveur " << i << " sur le port " << servers[i].getPort() << std::endl;
            for (size_t j = 0; j < i; j++) {
                if (servers[j].getSockfd() != -1)
                    close(servers[j].getSockfd());
            }
            return (1);
        }
    }
    return (start_server(servers));
}
