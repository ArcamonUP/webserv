/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:31:47 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/19 17:44:39 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

bool is_cgi(ServerConfig &conf, Request &req)
{
	std::string uri = req.getUri();
	size_t pos = uri.find('?');
	if (pos != std::string::npos)
		uri.resize(pos);
	int l_index = find_matching_location_index(conf, uri);
	if (l_index == -1 || conf.getLocations()[l_index].getCgiPath().empty())
		return (false);
	std::string tempUri = uri + conf.getLocations()[l_index].getCgiExtension();
	std::string	endCgiPath = conf.getLocations()[l_index].getCgiPath();
	if (endCgiPath.length() >= tempUri.length()) {
		endCgiPath = endCgiPath.substr(endCgiPath.length() - tempUri.length());
		if (tempUri == endCgiPath)
			return (true);
	}
	return (false);
}

char **init_cgi(Request &req, ServerConfig &conf)
{
    char **envp = new char*[7];
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

	int l_index = find_matching_location_index(conf, "/upload/");
	if (l_index == -1)
		envp[env_count] = NULL, env_count++;
	else
	{
		std::string upload_name = "UPLOAD_PATH=" + conf.getLocations()[l_index].getUploadPath();
		envp[env_count] = new char[upload_name.size() + 1];
		std::strcpy(envp[env_count], upload_name.c_str());
		env_count++;
	}
    
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

int  cgi_post(Request &req, int *input_pipe, int *pipefd)
{
		ssize_t written = 0;
		if (req.getMethod() == "POST") 
		{
			std::string body = req.getBody();
			written = write(input_pipe[1], body.c_str(), body.size());
			if (written == -1)
				std::perror("write");
		}
		if (close(pipefd[1]) == -1)
			std::perror("close pipefd[1] failed");
		if (close(input_pipe[0]) == -1)
			std::perror("close input_pipe[0] failed");
		if (close(input_pipe[1]) == -1)
			std::perror("close input_pipe[1] failed");
		if (written == -1)
			return 1;
		return 0;
}

void cgi_delete(char *script_arg, char *upload_status_arg, char *upload_path_arg, char **args)
{
		delete[] script_arg;
		if (upload_status_arg)
			delete[] upload_status_arg;
		if (upload_path_arg)
			delete[] upload_path_arg;
		delete[] args;
}

void cgi_output(std::string &output, int client_fd, char **envp, int *pipefd, std::string &uri)
{
		Response rep(200, "OK");
        rep.setRessourcePath(uri);
		rep.defineContentType();
		rep.setBody(output);
		std::string response = rep.getSerializedResponse();
		if (send(client_fd, response.c_str(), response.size(), 0) == -1)
			std::perror("send to client failed");
		free_tab(envp);
		if (close(pipefd[0]) == -1)
			std::perror("close pipefd[0] failed");
}

void cgi_child(int *input_pipe, int *pipefd, int client_fd, char **args, char **envp)
{
		close(input_pipe[1]), close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
			std::perror("dup2 STDOUT_FILENO failed");
        	_exit(EXIT_FAILURE);
    	}
		if (dup2(input_pipe[0], STDIN_FILENO) == -1) {
        	std::perror("dup2 STDIN_FILENO failed");
        	_exit(EXIT_FAILURE);
		}
		close(pipefd[1]);
    	close(input_pipe[0]);
		execve(args[0], args, envp);
		delete[] args;
		free_tab(envp);
		close(client_fd);
		std::perror("execve"), _exit(1);
}

char ** cgi_uploads(const std::string &uri, ServerConfig &conf, char *&upload_status_arg, \
				char *&upload_path_arg, char *python_path, char *script_arg)
{
	bool need_upload_args = false;

	if (uri == "/upload.py" || uri == "/list.py") 
	{
		need_upload_args = true;
		int l_index = find_matching_location_index(conf, "/upload/");
		if (l_index != -1)
		{
				if (conf.getLocations()[l_index].getUploadStatus()) 
				{
					upload_status_arg = new char[3];
					std::strcpy(upload_status_arg, "on");
				} 
				else 
				{	
					upload_status_arg = new char[4];
					std::strcpy(upload_status_arg, "off");
				}
				std::string up_path = conf.getLocations()[l_index].getUploadPath();
				if (!up_path.empty()) 
				{
					upload_path_arg = new char[up_path.size() + 1];
					std::strcpy(upload_path_arg, up_path.c_str());
				}
		}
	}

	char **args;
	if (need_upload_args && upload_status_arg && upload_path_arg) 
	{
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
	return args;
}

int cgi(Request &req, int client_fd, ServerConfig& conf)
{
	std::string uri = req.getUri();
	size_t pos = uri.find('?');
	if (pos != std::string::npos) 
		uri.resize(pos);
	int	l_index = find_matching_location_index(conf, uri);
	if (l_index == -1) {
		Response response(500, "Internal Server Error");
		std::string error_response = response.getSerializedResponse();
		send(client_fd, error_response.c_str(), error_response.size(), 0);
		return 1;
	}
	uri+= conf.getLocations()[l_index].getCgiExtension();
	std::string script_path = "srcs/cgi" + uri;
	char **envp = init_cgi(req, conf);
	if (!envp) {
		Response response(500, "Internal Server Error");
		std::string error_response = response.getSerializedResponse();
		send(client_fd, error_response.c_str(), error_response.size(), 0);
		return 1;
	}
	char *python_path = (char *)"/usr/bin/python3";
	char *script_arg = new char[script_path.size() + 1];
	std::strcpy(script_arg, script_path.c_str());
	char *upload_status_arg = NULL;
	char *upload_path_arg = NULL;

	char **args = cgi_uploads(uri, conf, upload_status_arg, upload_path_arg, python_path, script_arg);

	int pipefd[2], input_pipe[2];

	if (pipe(pipefd) == -1 || pipe(input_pipe) == -1)
		return std::perror("pipe"), 1;
	pid_t pid = fork();
	if (pid == -1)
		return std::perror("fork"), 1;
	if (pid == 0)
		cgi_child(input_pipe, pipefd, client_fd, args, envp);
	else {
		if (cgi_post(req, input_pipe, pipefd))
		{
			cgi_delete(script_arg, upload_status_arg, upload_path_arg, args);
			return (free_tab(envp), close(pipefd[0]), 1);
		}
		char buffer [4096];
		int count;
		std::string output;
		while((count = read(pipefd[0], buffer, sizeof(buffer))) > 0)
			output.append(buffer, count);
		if (count == -1)
		{
			std::perror("read from pipefd[0] failed");
			Response response(500, "Internal Server Error");
			std::string		error_response = response.getSerializedResponse();
			send(client_fd, error_response.c_str(), error_response.size(), 0);
		}
		cgi_delete(script_arg, upload_status_arg, upload_path_arg, args);
		if (child_status(pid, client_fd))
			return (free_tab(envp), close(pipefd[0]), 1);
		if (count != -1)
			cgi_output(output, client_fd, envp, pipefd, uri);
	}
	return 0;
}