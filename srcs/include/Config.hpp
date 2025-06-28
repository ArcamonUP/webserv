/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:25:42 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/28 12:36:38 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "ServerConfig.hpp"
# include <iostream>

class Config
{
	private:
		std::vector<ServerConfig>	servers;
		Config();
	public:
		Config(const std::string& path);
		Config(const Config &copy);
		~Config();
		Config &operator=(const Config &src);
		
		std::vector<ServerConfig>	getServer(void) const;
		
		void	validateConfiguration();
		
		class	InvalidFileException : public std::exception {
			public: 
				const char *what() const throw();
		};
};

std::ostream &operator<<(std::ostream &stream, const Config &conf);

#endif

