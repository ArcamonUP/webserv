#pragma once 

#include "WebServ.hpp"

#define TIME_OUT 30

class Connection;
class ServerConfig;

class ConnectionHandler
{
	private : 
		std::map<int, Connection*> connections;

	public :
		ConnectionHandler();
		~ConnectionHandler();

		void add_connection(int fd, ServerConfig* config);
		int handle_client_data(int fd);
		Connection* get_connection(int fd);
		void	remove_connection(int fd);
		void	clean_up_timed_out(int epoll_fd);
};

extern	ConnectionHandler connection_handler;
