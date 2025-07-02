/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:16:53 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/02 21:26:10 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <sstream>
#include <ctime>
#include <map>
#include <netinet/in.h>

template<typename T>
std::string	toString(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

//utils.cpp
int	make_not_blocking_socket(int fd);

//request.cpp
int	wait_request(int fd, sockaddr_in sockaddr);