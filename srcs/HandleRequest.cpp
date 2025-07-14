/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 11:05:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/14 15:28:52 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

std::map<std::string, MethodHandler> method_map;

// std::string get_request(int connection)
// {
// 	char buffer[4096];
// 	std::string result;

// 	while (true)
// 	{
// 		ssize_t bytesRead = recv(connection, buffer, sizeof(buffer), 0);
// 		if (bytesRead <= 0)
// 			break;
// 		result.append(buffer, bytesRead);
// 		if (result.find("\r\n\r\n") != std::string::npos)
// 			break;
// 	}
// 	return result;
// }

void free_env(char **envp)
{
    if (!envp) return;

    for (size_t i = 0; envp[i]; i++)
        delete[] envp[i];
    delete[] envp;
}

char **init_cgi(Request &req)
{
    char **envp = new char*[6];
    int env_count = 0;
    
    std::string method = "REQUEST_METHOD=" + req.getMethod();
    envp[env_count] = new char[method.size() + 1];
    std::strcpy(envp[env_count], method.c_str());
    env_count++;
    
    std::string content_type = "CONTENT_TYPE=" + req.getHeaderValue("Content-Type");
    envp[env_count] = new char[content_type.size() + 1];
    std::strcpy(envp[env_count], content_type.c_str());
    env_count++;
    
    std::string content_length = "CONTENT_LENGTH=" + req.getHeaderValue("Content-Length");
    envp[env_count] = new char[content_length.size() + 1];
    std::strcpy(envp[env_count], content_length.c_str());
    env_count++;
    
    std::string script_name = "SCRIPT_NAME=" + req.getUri();
    envp[env_count] = new char[script_name.size() + 1];
    std::strcpy(envp[env_count], script_name.c_str());
    env_count++;

    std::string uri = req.getUri();
    std::string query_string = "QUERY_STRING=";
    size_t pos = uri.find('?');
    if (pos != std::string::npos) {
        query_string += uri.substr(pos + 1);
    }
    envp[env_count] = new char[query_string.size() + 1];
    std::strcpy(envp[env_count], query_string.c_str());
    env_count++;
    
    envp[env_count] = NULL;
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

int cgi(Request &req, int client_fd, ServerConfig& conf)
{
	std::string uri = req.getUri();
	size_t pos = uri.find('?');
	if (pos != std::string::npos) {
		uri = uri.substr(0, pos);
	}
	
	std::string script_path = "srcs/cgi" + uri;
	char **envp = init_cgi(req);
	
	char *python_path = (char*)"/usr/bin/python3";
	char *script_arg = new char[script_path.size() + 1];
	std::strcpy(script_arg, script_path.c_str());
	
	char *upload_status_arg = NULL;
	char *upload_path_arg = NULL;
	
	bool need_upload_args = false;
	if (uri == "/upload.py" || uri == "/list.py") {
		need_upload_args = true;
		
		for (size_t i = 0; i < conf.getLocations().size(); i++) {
			if (conf.getLocations()[i].getPath() == "/upload/") {
				if (conf.getLocations()[i].getUploadStatus()) {
					upload_status_arg = new char[3];
					std::strcpy(upload_status_arg, "on");
				} else {
					upload_status_arg = new char[4];
					std::strcpy(upload_status_arg, "off");
				}
				
				std::string up_path = conf.getLocations()[i].getUploadPath();
				upload_path_arg = new char[up_path.size() + 1];
				std::strcpy(upload_path_arg, up_path.c_str());
				break;
			}
		}
	}
	
	char **args;
	if (need_upload_args && upload_status_arg && upload_path_arg) {
		args = new char*[5];
		args[0] = python_path;
		args[1] = script_arg;
		args[2] = upload_status_arg;
		args[3] = upload_path_arg;
		args[4] = NULL;
	} else {
		args = new char*[3];
		args[0] = python_path;
		args[1] = script_arg;
		args[2] = NULL;
	}

	int pipefd[2], input_pipe[2];

	if (pipe(pipefd) == -1 || pipe(input_pipe) == -1)
		return std::perror("pipe"), 1;
	pid_t pid = fork();
	if (pid == -1)
		return std::perror("fork"), 1;
	if (pid == 0)
	{
		close(input_pipe[1]), close(pipefd[0]), close(client_fd);
		dup2(pipefd[1], STDOUT_FILENO), dup2(input_pipe[0], STDIN_FILENO);
		if (req.getMethod() == "POST"){
			std::string body = req.getBody();
			write(STDIN_FILENO, body.c_str(), body.size());
		}
		execve(args[0], args, envp);
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

		delete[] script_arg;
		if (upload_status_arg)
			delete[] upload_status_arg;
		if (upload_path_arg)
			delete[] upload_path_arg;
		delete[] args;
		
		if (child_status(pid, client_fd))
			return (free_env(envp), close(client_fd), close(pipefd[0]), 1);

		std::string response =
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: " + toString(output.size()) + "\r\n"
			"Connection: close\r\n"
			"\r\n" +  output;
		send(client_fd, response.c_str(), response.size(), 0);
		free_env(envp), close(pipefd[0]);
	}
	return 0;
}

bool	is_cgi(ServerConfig& conf, Request& req)
{
	std::string uri = req.getUri();
	size_t pos = uri.find('?');
	if (pos != std::string::npos)
		uri = uri.substr(0, pos);

	const std::vector<LocationConfig>& locations = conf.getLocations();
	for (size_t i = 0; i < locations.size(); i++) {
		const std::string& cgi_path = locations[i].getCgiPath();
		if (!cgi_path.empty()) {
			size_t cgi_pos = cgi_path.find_last_of("/");
			if (cgi_pos != std::string::npos) {
				std::string script_name = cgi_path.substr(cgi_pos);
				if (uri == script_name)
					return true;
			}
		}
	}
	return false;
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
		response->setBody(get_custom_error_page(conf, 501));
		return (response);
	}
}

int	handle_request(epoll_event *events, ServerConfig& conf)
{
	int client_fd = events->data.fd;
	int behaviour = connection_handler.handle_client_data(client_fd);
	if (behaviour == ERROR || behaviour == REQUEST_INCOMPLETE)
		return (behaviour);
	Connection * connection = connection_handler.get_connection(client_fd);
	if (!connection)
		return (ERROR);
	std::string serialized_request = connection->get_request();
	Request	request(serialized_request);
	if (request.getError())
	{
		connection->reset();
		return (ERROR);
	}

	Response*	response;
	std::string serialized_response;
	if (is_cgi(conf, request))
	{
		connection->reset();
		if (cgi(request, client_fd, conf) != 0)
			return (ERROR);
	}
	else 
	{
		response = handle_action(conf, request);
		serialized_response = response->getSerializedResponse();
		send(client_fd, serialized_response.c_str(), serialized_response.size(), NO_FLAGS);
		connection->reset();
		std::cout << "\033[35mA response has been sent to client " << client_fd - 5 << ".\033[0m" << std::endl;
		if (response->getHeaderValue("connection") == "keep-alive")
			return (delete (response), KEEP_ALIVE);
		std::cout << "\033[33mConnection with client " << client_fd - 5 << " has been closed.\033[0m" << std::endl;
		return (delete (response), CLOSE_CONNECTION);
	}
	std::cout << "\033[34mA CGI response has been sent to client " << client_fd - 5 << ".\033[0m" << std::endl;
	return (SUCCESS);
}
