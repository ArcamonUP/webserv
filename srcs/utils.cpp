/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:17:45 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/24 14:59:10 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Webserv.hpp"
#include <fcntl.h>

int	make_not_blocking_socket(int fd)
{
	int	flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		return (-1);
	if ((flags & O_NONBLOCK) != 0)
		return (0);
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		return (-1);
	return (0);
}

std::string int_to_string(size_t value) 
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
