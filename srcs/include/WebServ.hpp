/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:16:53 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/10 13:57:00 by kbaridon         ###   ########.fr       */
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

//AutoIndex.cpp
std::string generate_autoindex(const std::string& directory_path, const std::string& uri_path);

//Errors.cpp
std::string get_custom_error_page(ServerConfig& conf, int error_code);
std::string get_default_error_page(int error_code);
Response*	handle_all_exceptions(ServerConfig conf);
Response*	handle_error_buttons(ServerConfig conf, std::string uri);
bool		is_button_error(const Request &request);

//HandleMethod.cpp
Response*	HandleDELETE(ServerConfig conf, const Request& request);
Response*	HandleGET(ServerConfig conf, const Request& request);
Response*	HandleHEAD(ServerConfig conf, const Request& request);
Response*	HandlePOST(ServerConfig conf, const Request& request);

//HandleRequest.cpp
int		handle_request(epoll_event *events, ServerConfig& conf);

//InitServers.cpp
int		create_server_socket(int sockfd, ServerConfig& server_config);
Config	init(int ac, char **av);

//RequestUtils.cpp
std::string get_first_word(std::string &message);
std::string get_next_line(std::string &message);
double get_http_version(std::string &message);
double str_to_double(const std::string &s);

//Traductor.cpp
std::string ft_traductor(const std::string& encoded_str);

//Utils.cpp
bool		endsWith(const std::string &str, const std::string &suffix);
int find_matching_location_index(ServerConfig& conf, const std::string& uri);
int			ft_atoi(const std::string value);
std::string get_file_content(const std::string& file_path);
void		initMethodMap();
bool		is_all_digit(std::string str);
int			make_not_blocking_socket(int fd);
std::string	trim(const std::string &s);

//UtilsGet.cpp
std::string build_file_path(ServerConfig& conf, const std::string& uri);
Response*	handle_directory_request(ServerConfig& conf, const std::string& file_path, const std::string& uri, int location_index);
Response*	handle_download_request(ServerConfig conf, std::string uri);
Response*	handle_file_request(const std::string& file_path);
Response*	handle_stopserv_request(ServerConfig& conf);

//WaitRequest.cpp
int		wait_multiple_servers(std::vector<ServerConfig>& servers);

// Variable globale pour l'arrêt propre
extern bool g_signal;