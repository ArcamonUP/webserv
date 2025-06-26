/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:16:53 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/26 15:05:10 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <netinet/in.h>
# include <iostream>

//utils.cpp
int			make_not_blocking_socket(int fd);
bool		endsWith(const std::string &str, const std::string &suffix);
std::string	trim(const std::string &s);

//request.cpp
int		wait_request(int fd, sockaddr_in sockaddr);

#endif