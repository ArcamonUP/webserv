/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:16:53 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/03 02:30:48 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <sys/epoll.h>
#include <cstdio>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <sstream>
#include <ctime>
#include <map>
#include <netinet/in.h>

#include "Message.hpp"
#include "Request.hpp"
#include "Response.hpp"

# define PORTS 8080
# define MAX_EVENTS 32
# define NO_FLAGS 0

template<typename T>
std::string	toString(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

typedef Response* (*MethodHandler)(const Request& request);
extern	std::map<std::string, MethodHandler> method_map;

//Handle_Method.cpp
Response*	HandleHEAD(const Request& request);
Response*	HandleGET(const Request& request);
Response*	HandlePOST(const Request& request);
Response*	HandleDELETE(const Request& request);

//utils.cpp
void		initMethodMap();
int			make_not_blocking_socket(int fd);

//wait_request.cpp
std::string get_file_body(const Request& request);

//request.cpp
int			wait_request(int fd, sockaddr_in sockaddr);