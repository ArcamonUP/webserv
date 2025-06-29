/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:02:17 by pmateo            #+#    #+#             */
/*   Updated: 2025/06/29 21:21:21 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void	Response::process()
{
	
}

void	Response::setStatus(const int status)
{
	this->_status = status;
}

int	Response::getStatus() const
{
	return (this->_status);
}

const char *	Response::getSerializedResponse() const
{
	
}

void	Response::initBuilders()
{
	
}

Response::Response() {}

Response::Response(const Response &copy)
{
	*this = copy;
}

Response::~Response() {}

Response&	Response::operator=(const Response& src)
{
	if (this != &src)
	{
		Message::operator=(src);
		this->_status = src._status;
	}
	return (*this);
}