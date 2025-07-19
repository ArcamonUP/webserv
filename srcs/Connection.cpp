/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:01:14 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/19 16:08:51 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "Connection.hpp"

Connection::Connection(int client_fd, ServerConfig* config): fd(client_fd), last_activity(time(NULL)), server_config(config) {}

Connection::~Connection() {};

const ServerConfig* Connection::getServer(void) {
	return (this->server_config);
}

bool	Connection::get_data() {
	char buffer[8196];
	memset(buffer, 0, sizeof(buffer));
	ssize_t bytes = recv(fd, buffer, sizeof(buffer), MSG_DONTWAIT);
	if (bytes <= 0)
		return false;
	part_request.append(buffer, bytes);
	last_activity = time(NULL);
	return true;
}

bool	Connection::is_request_complete() const {
	if (part_request.find("\r\n\r\n") != std::string::npos)
		return (true);
	else
		return (false);
}

const std::string& Connection::get_request() const {
	return (this->part_request);
}
void	Connection::reset() {
	part_request.clear();
}

bool Connection::is_timed_out(int timeout) const {
	return ((time(NULL) - last_activity) > timeout);
}
