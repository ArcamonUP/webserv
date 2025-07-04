/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Answer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 11:05:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/04 13:41:59 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"
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

// void homepage(epoll_event *events, ServerConfig conf)
// {
// 	int client_fd = events->data.fd;

// 	std::string request = get_request(client_fd);
// 	std::ifstream file((conf.getRoot() + conf.getIndex()).c_str());
// 	if (!file.is_open())
// 	{
// 		std::string error_response =
// 			"HTTP/1.1 500 Internal Server Error\r\n"
// 			"Content-Type: text/plain\r\n"
// 			"Content-Length: 21\r\n"
// 			"Connection: close\r\n"
// 			"\r\n"
// 			"Erreur serveur interne";
// 		send(client_fd, error_response.c_str(), error_response.size(), 0);
// 		close(client_fd);
// 		return;
// 	}
// 	std::istreambuf_iterator<char> begin(file);
// 	std::istreambuf_iterator<char> end;
// 	std::string html_body(begin, end);
					
// 	file.close();

// 	std::ostringstream oss;
// 	oss << html_body.size();

// 	std::string response =
// 	"HTTP/1.1 200 OK\r\n"
// 	"Content-Type: text/html\r\n"
// 	"Content-Length: " + oss.str() + "\r\n"
// 	"connecton: close\r\n"
// 	"\r\n" + 
// 	html_body;

// 	send(client_fd, response.c_str(), response.size(), NO_FLAGS);
// 	close(client_fd);
// }

bool	is_cgi(ServerConfig conf, Request req)
{
	std::string	p1, p2, p3;

	p1 = conf.getLocations()[1].getCgiPath();
	p2 = conf.getLocations()[2].getCgiPath();
	p3 = conf.getLocations()[3].getCgiPath();

	std::string uri = req.getUri();
	std::cout << p1.substr(1) << " TESSSSSSSSSSSSSSSSSSSSSSSSST " << uri << std::endl;
	return (uri == p1.substr(1) || uri == p2.substr(1) || uri == p3.substr(1));
}

Response*	handle_action(ServerConfig& conf, Request& request)
{
	Response *response;
	std::map<std::string, MethodHandler>::const_iterator it;
	std::string method_request;
	 
	method_request = request.getMethod();
	it = method_map.find(method_request);
	if (it != method_map.end())
	{
		response = (*method_map[method_request])(conf, request);
		return (response);
	}
	else
	{
		response = new Response(501, "Not Implemented");
		return (response);
	}
}

int	handle_request(epoll_event *events, ServerConfig& conf)
{
	Response*	response;
	std::string serialized_response;
	int client_fd = events->data.fd;

	std::string serialized_request = get_request(client_fd);
	std::cout << "serialized request : \n" << serialized_request << std::endl;
	
	Request	request(serialized_request);
	std::cout << "parsed request : \n" << request << std::endl;


	if (is_cgi(conf, request))
	{
	
		std::string content_type = "CONTENT_TYPE=" + request.getHeaderValue("Content-Type");

		std::string content_length = "CONTENT_LENGTH=" + request.getHeaderValue("Content-Length");
		std::cout << content_length << "  BBBBBBBBBBBBBBBBBBBBB  " << content_type << "          " << std::endl;
	
		if (cgi(request, client_fd) == 0)
			return (0);
		else 
			return (1);
	}
	else if (request.getMethod() == "POST" && request.getUri() == "/stop_server")
	{
		std::string stop_response = 
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: 84\r\n"
			"Connection: close\r\n"
			"\r\n"
			"<html><body><h1>Server i stopping...</h1><p>Bye !</p></body></html>";
		send(client_fd, stop_response.c_str(), stop_response.size(), NO_FLAGS);
		close(client_fd);
		return (2);
	}
	else {
		
		response = handle_action(conf, request);
		serialized_response = response->getSerializedResponse();
		
		send(client_fd, serialized_response.c_str(), serialized_response.size(), NO_FLAGS);
		close(client_fd), delete (response);
		return (1);
	}
	return (1);
}
