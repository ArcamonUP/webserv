/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:16:53 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/08 14:52:43 by kbaridon         ###   ########.fr       */
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
int find_matching_location_index(ServerConfig& conf, const std::string& uri);
bool has_custom_error_page(ServerConfig& conf, int error_code);
std::string get_default_error_page(int error_code);
std::string get_custom_error_page(ServerConfig& conf, int error_code);

//AutoIndex.cpp
std::string generate_autoindex_header(const std::string& uri_path);
std::vector<std::pair<std::string, bool> > collect_directory_entries(const std::string& directory_path, const std::string& uri_path);
void sort_directory_entries(std::vector<std::pair<std::string, bool> >& entries);
std::string generate_entry_link_path(const std::string& name, bool is_dir, const std::string& uri_path);
std::string generate_autoindex(const std::string& directory_path, const std::string& uri_path);

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