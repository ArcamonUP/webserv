/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:16:53 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/04 11:06:42 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "Config.hpp"
# include <netinet/in.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream> 
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <cstdio>

//utils.cpp
int			make_not_blocking_socket(int fd);
bool		endsWith(const std::string &str, const std::string &suffix);
std::string	trim(const std::string &s);
int			ft_atoi(const std::string value);
bool		is_all_digit(std::string str);

//request.cpp
int		wait_multiple_servers(std::vector<ServerConfig>& servers);
std::string int_to_string(size_t value);

//initServers.cpp
Config	init(int ac, char **av);
int		create_server_socket(int sockfd, ServerConfig& server_config);

//answer.cpp
int		answer(epoll_event *events, ServerConfig& conf, int epoll_fd);

// Variable globale pour l'arrêt propre: peut etre a enlever
extern bool g_signal;
#endif