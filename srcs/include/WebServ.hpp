/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:16:53 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/04 16:53:24 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "Message.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include <netinet/in.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream> 
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <string>
#include <unistd.h>
#include <sys/epoll.h>
#include <cstdio>
#include <cstring>

# define MAX_EVENTS 512
# define NO_FLAGS 0

template<typename T>
std::string	toString(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

typedef Response* (*MethodHandler)(ServerConfig conf, const Request& request);
extern	std::map<std::string, MethodHandler> method_map;

//utils.cpp
void		initMethodMap();
int			make_not_blocking_socket(int fd);
bool		endsWith(const std::string &str, const std::string &suffix);
std::string	trim(const std::string &s);
int			ft_atoi(const std::string value);
bool		is_all_digit(std::string str);
std::string get_file_content(const std::string& file_path);

//request.cpp
int		wait_multiple_servers(std::vector<ServerConfig>& servers);
std::string int_to_string(size_t value);

//initServers.cpp
Config	init(int ac, char **av);
int		create_server_socket(int sockfd, ServerConfig& server_config);

//answer.cpp
int		handle_request(epoll_event *events, ServerConfig& conf);

// Variable globale pour l'arrêt propre: peut etre a enlever
extern bool g_signal;

//wait_request.cpp
void 		homepage(epoll_event *events, ServerConfig conf);

//Handle_Method.cpp
Response*	HandleHEAD(ServerConfig conf, const Request& request);
Response*	HandleGET(ServerConfig conf, const Request& request);
Response*	HandlePOST(ServerConfig conf, const Request& request);
Response*	HandleDELETE(ServerConfig conf, const Request& request);