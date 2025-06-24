/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:06:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/24 15:47:47 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>

class Request
{
	private:
		std::string method;
		std::string	uri;
		double		http_version;
		bool		error;
		Request();
	public:
		Request(std::string request);
		Request(const Request &copy);
		~Request();
		Request	&operator=(const Request &src);
};

#endif
