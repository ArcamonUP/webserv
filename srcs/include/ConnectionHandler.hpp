#pragma once 

#include "WebServ.hpp"

#define TIME_OUT 75
#define MAX_CONNECTIONS 1000

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
		size_t	get_connection_count() const;
};

extern	ConnectionHandler connection_handler;
