/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:25:42 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/26 16:46:58 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "ServerConfig.hpp"
# include <iostream>

class Config
{
	private:
		ServerConfig	server;
		Config();
	public:
		Config(const std::string& path);
		Config(const Config &copy);
		~Config();
		Config &operator=(const Config &src);
		
		ServerConfig	getServer(void) const;
		
		class	InvalidFileException : public std::exception {
			public: virtual const char *what() const throw();
		};
};

//std::ostream &operator<<(std::ostream &stream, const Config &conf);

#endif

