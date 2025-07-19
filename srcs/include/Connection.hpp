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
		Connection(int client_fd, ServerConfig* config);
		~Connection();

		const ServerConfig*	getServer(void);
		const std::string&	get_request() const;
		bool				get_data();

		bool				is_request_complete() const;
		bool				is_timed_out(int timeout) const;

		void				reset();
};
