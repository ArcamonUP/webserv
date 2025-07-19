/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:30:00 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/18 11:19:08 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <iomanip>
#include <algorithm>

std::string generate_autoindex_header(const std::string& uri_path)
{
	std::ostringstream html;
	html << "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>Index de " << uri_path << "</title>";
	html << "<style>body{font-family:'Segoe UI',sans-serif;margin:0;background:linear-gradient(135deg,#667eea,#764ba2);min-height:100vh}";
	html << ".container{background:white;border-radius:15px;box-shadow:0 20px 40px rgba(0,0,0,0.1);padding:40px;max-width:900px;margin:20px auto}";
	html << "h1{color:#333;text-align:center;margin-bottom:30px}table{width:100%;border-collapse:collapse;margin-top:20px;background:#f8f9fa;border-radius:10px;overflow:hidden}";
	html << "th,td{text-align:left;padding:15px;border-bottom:1px solid #e9ecef}th{background:linear-gradient(45deg,#667eea,#764ba2);color:white;font-weight:500}";
	html << "tr:hover{background:#e3f2fd}a{color:#667eea;text-decoration:none;font-weight:500}a:hover{text-decoration:underline}";
	html << ".btn{background:linear-gradient(45deg,#667eea,#764ba2);color:white;padding:12px 24px;border-radius:25px;text-decoration:none;display:inline-block;margin:5px}";
	html << ".btn:hover{transform:translateY(-2px)}.links{text-align:center;margin-top:30px}</style></head><body>";
	html << "<div class=\"container\"><h1>📁 Index de " << uri_path << "</h1>";
	
	if (uri_path != "/")
		html << "<div class=\"links\"><a href=\"/\" class=\"btn\">🏠 Retour a l'accueil</a></div>";
	html << "<table><thead><tr><th>Nom</th><th>Taille</th><th>Derniere modification</th></tr></thead><tbody>";
	return html.str();
}

std::vector<std::pair<std::string, bool> > collect_directory_entries(const std::string& directory_path, const std::string& uri_path)
{
	DIR *dir = opendir(directory_path.c_str());
	if (!dir) {
		throw Response::InternalServerErrorException();
	}

	std::vector<std::pair<std::string, bool> > entries;
	const struct dirent *entry;

	entries.push_back(std::make_pair(".", true));
	if (uri_path != "/")
		entries.push_back(std::make_pair("..", true));

	while ((entry = readdir(dir)) != NULL) {
		std::string name = entry->d_name;
		if (name == "." || name == "..")
			continue;
		std::string full_path = directory_path + "/" + name;
		struct stat file_stat;
		bool is_dir = false;
		if (stat(full_path.c_str(), &file_stat) == 0)
			is_dir = S_ISDIR(file_stat.st_mode);
		entries.push_back(std::make_pair(name, is_dir));
	}
	closedir(dir);
	return entries;
}

void sort_directory_entries(std::vector<std::pair<std::string, bool> >& entries)
{
	for (size_t i = 0; i < entries.size(); i++) {
		for (size_t j = i + 1; j < entries.size(); j++) {
			bool swap_needed = false;
			if (entries[i].second != entries[j].second) {
				swap_needed = entries[j].second > entries[i].second;
			} else
				swap_needed = entries[i].first > entries[j].first;
			if (swap_needed) {
				std::pair<std::string, bool> temp = entries[i];
				entries[i] = entries[j];
				entries[j] = temp;
			}
		}
	}
}

std::string generate_entry_link_path(const std::string& name, bool is_dir, const std::string& uri_path)
{
	std::string link_path;
	
	if (name == ".")
		link_path = uri_path;
	else if (name == "..") {
		std::string parent_path = uri_path;
		if (!parent_path.empty() && parent_path[parent_path.length() - 1] == '/')
			parent_path.resize(parent_path.length() - 1);
		size_t last_slash = parent_path.find_last_of('/');
		link_path = (last_slash != std::string::npos) ? parent_path.substr(0, last_slash + 1) : "/";
	} else {
		link_path = uri_path;
		if (link_path.empty() || link_path[link_path.length() - 1] != '/') link_path += "/";
		link_path += name;
		if (is_dir) link_path += "/";
	}
	return link_path;
}

std::string generate_file_entry_html(const std::string& name, bool is_dir, const std::string& link_path, const std::string *directory_path)
{
	std::string			fullpath, size_str = "-", time_str = "-";
	std::ostringstream	html;
	struct stat			fileStat;

	if (is_dir)
		html << "<tr><td> 📁 <a href=\"" << link_path << "\">" << name << "</a></td><td>-</td><td>-</td></tr>";
	else
	{
		fullpath = *directory_path + "/" + name;
		if (stat(fullpath.c_str(), &fileStat) == 0)
		{
			off_t size = fileStat.st_size;
			std::ostringstream ss;
			if (size < 1024) ss << size << " B";
			else if (size < 1024 * 1024) ss << std::fixed << std::setprecision(1) << (double)size / 1024 << " KB";
			else ss << std::fixed << std::setprecision(1) << (double)size / (1024 * 1024) << " MB";
			size_str = ss.str();
			
			char time_buf[64];
			const struct tm *tm_info = localtime(&fileStat.st_mtime);
			strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M", tm_info);
			time_str = time_buf;
		}
		html << "<tr><td> 📄 " << name << "</td><td>" << size_str << "</td><td>" << time_str << "</td></tr>";
	}
	return html.str();
}

std::string generate_autoindex(const std::string& directory_path, const std::string& uri_path)
{
	std::ostringstream html;
	html << generate_autoindex_header(uri_path);

	std::vector<std::pair<std::string, bool> > entries = collect_directory_entries(directory_path, uri_path);
	sort_directory_entries(entries);

	for (std::vector<std::pair<std::string, bool> >::iterator it = entries.begin(); it != entries.end(); ++it) {
		std::string name = it->first;
		bool is_dir = it->second;
		std::string link_path = generate_entry_link_path(name, is_dir, uri_path);

		html << generate_file_entry_html(name, is_dir, link_path, &directory_path);
	}
	html << "</tbody></table></div></body></html>";
	return html.str();
}
