/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:06:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/25 14:54:42 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <vector>

class Request
{
	private:
		std::string method;
		std::string	uri;
		double		http_version;
		std::vector<std::pair<std::string, std::string> > headers;
		std::string	body;
		bool		error;
		Request();
	public:
		Request(std::string request);
		Request(const Request &copy);
		~Request();
		Request	&operator=(const Request &src);

		std::string	getMethod() const;
		std::string getUri() const;
		double		getHttpVersion() const;
		std::string	getHeaderMap() const;
		std::string	getHeaderValue(std::string key) const;
		std::string	getBody() const;
		bool		getError() const;
};

std::ostream &operator<<(std::ostream &stream, Request const &request);

#endif
