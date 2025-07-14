/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:25:27 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/10 15:26:12 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <vector>
#include <iostream>

class LocationConfig
{
	private:
		std::string					path;
		std::string					root;
		std::string					index;
		std::string					redirect;
		bool						autoindex;
		std::string					cgi_extension;
		std::string					cgi_path;
		bool						upload_status;
		std::string					upload_path;
		size_t						max_size_body;
		std::vector<std::string>	allowed_methods;

	public:
		LocationConfig();
		void	setPath(const std::string& p);
		void	setRoot(const std::string& root);
		void	setIndex(const std::string& i);
		void	setRedirect(const std::string& redir);
		void	setAutoIndex(const bool autoi);
		void	setCgiExtension(const std::string& cgi_e);
		void	setCgiPath(const std::string& cgi_p);
		void	setUploadStatus(bool st);
		void	setUploadPath(const std::string& up);
		void	setMaxSizeBody(size_t max);
		void	setAllowedMethods(const std::vector<std::string>& m);

		const std::string&				getPath() const;
		const std::string&				getRoot() const;
		const std::string&				getIndex() const;
		const std::string&				getRedirect() const;
		bool							getAutoIndex() const;
		const std::string&				getCgiExtension() const;
		const std::string&				getCgiPath() const;
		bool							getUploadStatus() const;
		const std::string&				getUploadPath() const;
		size_t							getMaxSizeBody() const;
		const std::vector<std::string>&	getAllowedMethods() const;
};

std::ostream& operator<<(std::ostream& os, const LocationConfig& src);

#endif

