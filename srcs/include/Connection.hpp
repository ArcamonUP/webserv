#pragma once

#include "WebServ.hpp"

class Connection
{
private:
	int fd;
	time_t	last_activity;
	ServerConfig* server_config;
	std::string part_request;
	
public:
	Connection(int client_fd, ServerConfig* config) : fd(client_fd), last_activity(time(NULL)), server_config(config) {}
	~Connection() {}

	const ServerConfig* getServer(void) {
		return (this->server_config);
	}
	bool	get_data()
	{
		char buffer[8196];
		ssize_t bytes = recv(fd, buffer, sizeof(buffer), MSG_DONTWAIT);
		if (bytes < 0)
			return false;
		part_request.append(buffer, bytes);
		last_activity = time(NULL);
		return true;
	}

	bool	is_request_complete() const
	{
		if (part_request.find("\r\n\r\n") != std::string::npos)
			return (true);
		else
			return (false);
	}

	const std::string& get_request() const 
	{
		return part_request;
	}

	void	reset() 
	{
		part_request.clear();
	}

	bool is_timed_out(int timeout) const
	{
		return ((time(NULL) - last_activity) > timeout);
	}
};
