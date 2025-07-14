/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:25:54 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/14 13:05:22 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>
#include <map>
#include <netinet/in.h>
#include "LocationConfig.hpp"

class ServerConfig
{
	private:
		std::string					host;
		int							port;
		size_t						max_size_body;
		std::string					root;
		std::string					index;
		std::map<int, std::string>	error_pages;
		std::vector<LocationConfig>	locations;
		int							sockfd;
		sockaddr_in					sockaddr;
	public:
		ServerConfig();
		void	setHost(const std::string &ip);
		void	setPort(int p);
		void	setMaxSizeBody(size_t max);
		void	setRoot(const std::string& r);
		void	setIndex(const std::string& i);
		void	setErrorPages(const std::map<int, std::string>& e);
		void	addErrorPage(int code, std::string page);
		void	setLocations(const std::vector<LocationConfig>& locs);
		void	addLocation(const LocationConfig loc);
		void	setSockfd(int fd);
		void	setSockaddr(const sockaddr_in& addr);

		const std::string&					getHost() const;
		int									getPort() const;
		size_t								getMaxSizeBody() const;
		const std::string&					getRoot() const;
		const std::string&					getIndex() const;
		const std::map<int, std::string>&	getErrorPages() const;
		const std::vector<LocationConfig>	getLocations() const;
		int									getSockfd() const;
		const sockaddr_in&					getSockaddr() const;
};

std::ostream& operator<<(std::ostream& os, const ServerConfig& srv);

#endif

