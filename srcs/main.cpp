/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:31:38 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/14 15:27:13 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

bool g_signal = false;

int start_server(std::vector<ServerConfig>& servers)
{
    int result;

    if (servers.empty())
    {
        std::cerr << "\033[31mNo servers configured. Shutting down the program...\033[0m" << std::endl;
        return (1);
    }
    std::cout << "\033[32mAll servers have started. Awaiting connections...\033[0m" << std::endl;
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
        return (FAILURE);
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
            std::cerr << "\033[31mUnable to initialize server " << i << " on port " << servers[i].getPort() << "\033[0m" << std::endl;
            for (size_t j = 0; j < i; j++) {
                if (servers[j].getSockfd() != -1)
                    close(servers[j].getSockfd());
            }
            return (FAILURE);
        }
    }
    return (start_server(servers));
}
