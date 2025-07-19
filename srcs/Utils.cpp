/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:17:45 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/19 13:41:38 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

void	initMethodMap()
{
	method_map["HEAD"] = &HandleHEAD;
	method_map["GET"] = &HandleGET;
	method_map["POST"] = &HandlePOST;
	method_map["DELETE"] = &HandleDELETE;
}

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

int	ft_atoi(const std::string value)
{
	std::size_t	i = 0;
	int			sign = 1;
	long		result = 0;

	while (i < value.length() && (value[i] == ' ' || value[i] == '\t' || value[i] == '\n'
		|| value[i] == '\v' || value[i] == '\f' || value[i] == '\r'))
		i++;
	if (i < value.length() && (value[i] == '-' || value[i] == '+'))
	{
		if (value[i] == '-')
			sign = -1;
		i++;
	}
	while (i < value.length() && value[i] >= '0' && value[i] <= '9')
	{
		result = result * 10 + (value[i] - '0');
		i++;
	}

	return (static_cast<int>(sign * result));
}

bool	is_all_digit(const std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
	}
	return (true);
}

std::string get_file_content(const std::string& file_path)
{
	std::ifstream file(file_path.c_str());
	if (!file.is_open() || file.fail())
		throw Response::InternalServerErrorException();	
	std::stringstream	buffer;
	std::string file_content;

	buffer << file.rdbuf();
	file.close();
	file_content = buffer.str();
	return (file_content);
}

int find_matching_location_index(ServerConfig& conf, const std::string& uri)
{
	const std::vector<LocationConfig>& locations = conf.getLocations();
	int best_match_index = -1;
	size_t best_match_length = 0;

	for (size_t i = 0; i < locations.size(); i++) {
		const std::string& location_path = locations[i].getPath();
		if (uri.find(location_path) == 0) {
			if (location_path.length() > best_match_length) {
				best_match_index = (int)i;
				best_match_length = location_path.length();
			}
		}
	}
	
	return best_match_index;
}

bool    check_ip(const std::string &ip)
{
    int nb_dot = 0;
    size_t start = 0;

    for (size_t i = 0; i <= ip.length(); i++)
    {
        if (i == ip.length() || ip[i] == '.')
        {
            if (i == start)
                return (false);
            std::string segment = ip.substr(start, i - start);
            if (segment.length() > 3 || !is_all_digit(segment))
                return (false);
            if (ft_atoi(segment) > 255)
                return (false);
            if (ip[i] == '.')
                nb_dot++;
            start = i + 1;
        }
        else if (ip[i] < '0' || ip[i] > '9')
            return (false);
    }
    return (nb_dot == 3);
}

void free_tab(char **tab)
{
    if (!tab) return;

    for (size_t i = 0; tab[i]; i++)
        delete[] tab[i];
    delete[] tab;
}
