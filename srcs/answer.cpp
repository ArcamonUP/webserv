/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   answer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 11:05:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/04 11:37:14 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "Request.hpp"
#define NO_FLAGS 0

std::string get_request(int connection)
{
	char buffer[4096];
	std::string result;

	while (true)
	{
		ssize_t bytesRead = recv(connection, buffer, sizeof(buffer), 0);
		if (bytesRead <= 0)
			break;
		result.append(buffer, bytesRead);
		if (bytesRead < (ssize_t)sizeof(buffer))
			break ;
	}
	return result;
}

int cgi(Request &req, int client_fd)
{
	std::string uri = "srcs/cgi" + req.getUri();
	std::string method = "REQUEST_METHOD=" + req.getMethod();
	std::string content_type = "CONTENT_TYPE=" + req.getHeaderValue("Content-Type");
	std::string content_length = "CONTENT_LENGTH=" + req.getHeaderValue("Content-Length");
	std::string script_name = "SCRIPT_NAME=" + req.getUri();

	std::cout << content_length << "     " << content_type << "          " << req.getBody() << std::endl;

	char *envp[] = {
		(char *)method.c_str(),
		(char *)content_type.c_str(),
		(char *)content_length.c_str(),
		(char *)script_name.c_str(),
		NULL
	};
	
	char *argv[] = {(char*)"/usr/bin/python3", (char*)uri.c_str(), NULL};

	int pipefd[2];
	int input_pipe[2];

	if (pipe(pipefd) == -1 || pipe(input_pipe) == -1)
		return std::perror("pipe"), 1;
	
	pid_t pid = fork();
	if (pid == -1)
		return std::perror("fork"), 1;
	if (pid == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		dup2(input_pipe[0], STDIN_FILENO);
		close(input_pipe[1]); 
		close(pipefd[0]);
		if (req.getMethod() == "POST")
		{
			std::string body = req.getBody();
			write(STDIN_FILENO, body.c_str(), body.size());
		}
		execve(argv[0], argv, envp);
	}
	else {
		close(pipefd[1]);
		close(input_pipe[0]);

		if (req.getMethod() == "POST") 
		{
			std::string body = req.getBody();
			write(input_pipe[1], body.c_str(), body.size());
		}
		close(input_pipe[1]);

		
		char buffer [4096];
		size_t count;
		std::string output;
		while((count = read(pipefd[0], buffer, sizeof(buffer))) > 0)
		{
			output.append(buffer, count);
		}
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status)) 
		{
			int code_retour = WEXITSTATUS(status);
			std::cout << "Error children : " << code_retour << std::endl;
			if(code_retour > 0) {

				std::string error_response =
				"HTTP/1.1 500 Internal Server Error\r\n"
				"Content-Type: text/plain\r\n"
				"Content-Length: 21\r\n"
				"Connection: close\r\n"
				"\r\n"
				"Erreur serveur interne";
				send(client_fd, error_response.c_str(), error_response.size(), 0);
				close(client_fd);
				return 1;
			}
    	}
		std::cout << output << std::endl;

		std::string response =
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: " + int_to_string(output.size()) + "\r\n"
			"Connection: close\r\n"
			"\r\n" +  output;
		send(client_fd, response.c_str(), response.size(), 0);
		close(client_fd);
	}
	return 0;
}

void homepage(epoll_event *events, ServerConfig conf)
{
	int client_fd = events->data.fd;

	std::string request = get_request(client_fd);
	std::ifstream file((conf.getRoot() + conf.getIndex()).c_str());
	if (!file.is_open())
	{
		std::string error_response =
			"HTTP/1.1 500 Internal Server Error\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: 21\r\n"
			"Connection: close\r\n"
			"\r\n"
			"Erreur serveur interne";
		send(client_fd, error_response.c_str(), error_response.size(), 0);
		close(client_fd);
		return;
	}
	std::istreambuf_iterator<char> begin(file);
	std::istreambuf_iterator<char> end;
	std::string html_body(begin, end);
					
	file.close();

	std::ostringstream oss;
	oss << html_body.size();

	std::string response =
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html\r\n"
	"Content-Length: " + oss.str() + "\r\n"
	"connecton: close\r\n"
	"\r\n" + 
	html_body;

	send(client_fd, response.c_str(), response.size(), NO_FLAGS);
	close(client_fd);
}

bool	is_cgi(ServerConfig conf, Request req)
{
	std::string	p1, p2, p3;
	size_t		n1, n2, n3;

	p1 = conf.getLocations()[1].getCgiPath();
	p2 = conf.getLocations()[2].getCgiPath();
	p3 = conf.getLocations()[3].getCgiPath();

	n1 = p1.find_last_of('/');
	n2 = p2.find_last_of('/');
	n3 = p3.find_last_of('/');

	std::string uri = req.getUri();
	return (uri == p1.substr(n1) || uri == p2.substr(n2) || uri == p3.substr(n3));
}

int answer(epoll_event *events, ServerConfig& conf, int epoll_fd)
{
	std::string response;
	int client_fd = events->data.fd;

	std::string request = get_request(client_fd);
	std::cout << "raw request : \n" << request << std::endl;
	Request curr_req(request);
	std::cout << "parsed request : \n" << curr_req << std::endl;

	if (is_cgi(conf, curr_req))
	{
			if (cgi(curr_req, client_fd) == 0)
				return (1);
	}
	else if (curr_req.getMethod() == "POST" && curr_req.getUri() == "/stop_server")
	{
		std::string stop_response = 
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: 120\r\n"
			"Connection: close\r\n"
			"\r\n"
			"<html><body><h1>Serveur arrêté</h1><p>Le serveur sur le port " 
			+ int_to_string(conf.getPort()) + 
			" a été arrêté avec succès.</p></body></html>";
		send(client_fd, stop_response.c_str(), stop_response.size(), NO_FLAGS);
		close(client_fd);
		
		if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, conf.getSockfd(), NULL) == -1) {
			std::perror("epoll_ctl DEL");
		}
		close(conf.getSockfd());
		
		std::cout << "Serveur sur port " << conf.getPort() << " arrêté avec succès" << std::endl;
		return (2);
	}
	else if (curr_req.getMethod() == "GET" && curr_req.getUri().empty())
		homepage(events, conf);
	else if (curr_req.getMethod() == "POST")
	{
		std::string data = curr_req.getBody();
		std::cout << data << std::endl;
		response =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: ";
		std::ostringstream oss;
		oss << data.length();
		response += oss.str();
		response += "\r\n";
		response += "Connection: close\r\n";
		response += "\r\n";
		response += data;
		send(client_fd, response.c_str(), response.size(), NO_FLAGS);
		close(client_fd);
	}
	else {
		close(client_fd);
	}
	return (1);
}
