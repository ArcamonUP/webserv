/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:16:53 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/24 11:46:35 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <netinet/in.h>

//utils.cpp
int	make_not_blocking_socket(int fd);

//request.cpp
int	wait_request(int fd, sockaddr_in sockaddr);

#endif