/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 11:05:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/07 11:17:04 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "Request.hpp"
#include "Response.hpp"
#define NO_FLAGS 0

std::map<std::string, MethodHandler> method_map;

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

void free_env(char **envp)
{
    if (!envp) return;

    for (size_t i = 0; envp[i]; ++i)
        delete[] envp[i];
    delete[] envp;
}

char **init_cgi(Request &req)
{
    std::vector<std::string> env_strings;
    env_strings.push_back("REQUEST_METHOD=" + req.getMethod());
    env_strings.push_back("CONTENT_TYPE=" + req.getHeaderValue("Content-Type"));
    env_strings.push_back("CONTENT_LENGTH=" + req.getHeaderValue("Content-Length"));
    env_strings.push_back("SCRIPT_NAME=" + req.getUri());

    char **envp = new char*[env_strings.size() + 1]; 

    for (size_t i = 0; i < env_strings.size(); ++i)
    {
        envp[i] = new char[env_strings[i].size() + 1];
        std::strcpy(envp[i], env_strings[i].c_str());
    }
    envp[env_strings.size()] = NULL;
    return envp;
}

int child_status(pid_t &pid, int client_fd)
{
		int 			status;
		std::string		error_response;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status)) 
		{
			int code_retour = WEXITSTATUS(status);
			if(code_retour > 0) {

				Response response(500, "Internal Server Error");
				error_response = response.getSerializedResponse();
				send(client_fd, error_response.c_str(), error_response.size(), 0);
				close(client_fd);
				return 1;
			}
    	}
		return (0);
}

int cgi(Request &req, int client_fd)
{
	std::string uri = "srcs/cgi" + req.getUri();
	char **envp = init_cgi(req);
	char *argv[] = {(char*)"/usr/bin/python3", (char*)uri.c_str(), NULL};

	int pipefd[2], input_pipe[2];

	if (pipe(pipefd) == -1 || pipe(input_pipe) == -1)
		return std::perror("pipe"), 1;
	pid_t pid = fork();
	if (pid == -1)
		return std::perror("fork"), 1;
	if (pid == 0)
	{
		close(input_pipe[1]), close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO), dup2(input_pipe[0], STDIN_FILENO);
		if (req.getMethod() == "POST"){
			std::string body = req.getBody();
			write(STDIN_FILENO, body.c_str(), body.size());
		}
		execve(argv[0], argv, envp);
		std::perror("execve"), _exit(1);
	}
	else {
		if (req.getMethod() == "POST") {
			std::string body = req.getBody();
			write(input_pipe[1], body.c_str(), body.size());
		}
		close(pipefd[1]), close(input_pipe[0]), close(input_pipe[1]);

		
		char buffer [4096];
		size_t count;
		std::string output;
		while((count = read(pipefd[0], buffer, sizeof(buffer))) > 0)
			output.append(buffer, count);

		if (child_status(pid, client_fd))
			return (free_env(envp), close(client_fd),1);
		std::cout << output << std::endl;

		std::string response =
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: " + toString(output.size()) + "\r\n"
			"Connection: close\r\n"
			"\r\n" +  output;
		send(client_fd, response.c_str(), response.size(), 0);
		free_env(envp), close(client_fd);
	}
	return 0;
}

bool	is_cgi(ServerConfig& conf, Request& req)
{
	std::string	p1, p2, p3;

	p1 = conf.getLocations()[1].getCgiPath();
	p2 = conf.getLocations()[2].getCgiPath();
	p3 = conf.getLocations()[3].getCgiPath();

	std::string uri = req.getUri();
	return (uri == p1.substr(10) || uri == p2.substr(10) || uri == p3.substr(10));
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
	Request	request(serialized_request);
	if (request.getError())
		return (1);
	std::cout << "parsed request : \n" << request << std::endl;

	if (is_cgi(conf, request))
	{
		if (cgi(request, client_fd) == 0)
			return (0);
	}
	else {
		response = handle_action(conf, request);
		serialized_response = response->getSerializedResponse();
		send(client_fd, serialized_response.c_str(), serialized_response.size(), NO_FLAGS);
		close(client_fd), delete (response);
		if (request.getUri() == "/stopserv")
			return (2);
	}
	return (1);
}
