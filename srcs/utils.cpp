/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:17:45 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/26 16:44:37 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
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

bool	endsWith(const std::string &str, const std::string &suffix)
{
	if (suffix.size() > str.size())
		return (false);
	return (str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0);
}

std::string trim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}
