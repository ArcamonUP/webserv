/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:31:15 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/19 14:17:41 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectionHandler.hpp"

ConnectionHandler::ConnectionHandler() {}

ConnectionHandler::~ConnectionHandler() {
	for (std::map<int, Connection*>::iterator it = connections.begin(); it != connections.end(); it++)
		delete it->second;
	connections.clear();
}

void ConnectionHandler::add_connection(int fd, ServerConfig* config) {
	if (connections.size() >= MAX_CONNECTIONS) {
		close(fd);
		return;
	}
	connections[fd] = new Connection(fd, config);
}

int ConnectionHandler::handle_client_data(int fd) {
	std::map<int, Connection*>::iterator it = connections.find(fd);
	if (it == connections.end())
		return (ERROR);
	Connection* conn = it->second;
	if (!conn->get_data())
		return (ERROR);
	else if (conn->is_request_complete())
		return (REQUEST_COMPLETE);
	else
		return (REQUEST_INCOMPLETE);
}

Connection* ConnectionHandler::get_connection(int fd) {
	std::map<int, Connection*>::const_iterator it = connections.find(fd);
	return ((it != connections.end()) ? it->second : NULL);
}

void	ConnectionHandler::remove_connection(int fd) {
	std::map<int, Connection*>::iterator it = connections.find(fd);
	if (it != connections.end())
	{
		delete it->second;
		connections.erase(it);
	}
}

void	ConnectionHandler::clean_up_timed_out(int epoll_fd) {
	std::map<int, Connection*>::iterator it;
	for (it = connections.begin(); it != connections.end();)
	{
		if (it->second->is_timed_out(TIME_OUT))
		{
			std::map<int, Connection*>::iterator next = it;
			int fd = it->first;
			delete it->second;
			++next;
			connections.erase(it);
			it = next;
			epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
			close(fd);
		}
		else
			++it;
	}
}

size_t ConnectionHandler::get_connection_count() const {
	return connections.size();
}
