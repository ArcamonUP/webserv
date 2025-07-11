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
		ConnectionHandler() {}
		~ConnectionHandler()
		{
			for (std::map<int, Connection*>::iterator it = connections.begin(); it != connections.end(); it++)
				delete it->second;
			connections.clear();
		}

		void add_connection(int fd, ServerConfig* config)
		{
			connections[fd] = new Connection(fd, config);
		}

		int handle_client_data(int fd)
		{
			std::map<int, Connection*>::iterator it = connections.find(fd);
			if (it == connections.end())
				return (-1);
			Connection* conn = it->second;
			if (!conn->get_data())
				return (-1);
			else if (conn->is_request_complete())
				return (1);
			else
				return (0);
		}

		Connection* get_connection(int fd)
		{
			std::map<int, Connection*>::const_iterator it = connections.find(fd);
			return ((it != connections.end()) ? it->second : NULL);
		}

		void	remove_connection(int fd)
		{
			std::map<int, Connection*>::iterator it = connections.find(fd);
			if (it != connections.end())
			{
				delete it->second;
				connections.erase(it);
			}
		}

		void	clean_up_timed_out(int epoll_fd)
		{
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

		void	clean_up_all_connections()
		{
			for (std::map<int, Connection*>::iterator it = connections.begin(); it != connections.end(); it++)
				delete it->second;
			connections.clear();
		}
};