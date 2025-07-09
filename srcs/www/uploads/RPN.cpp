/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:05:50 by kbaridon          #+#    #+#             */
/*   Updated: 2025/07/01 12:46:27 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN() {}

RPN::RPN(const RPN &copy) {
	RPN::operator=(copy);
}

RPN::~RPN() {}

RPN &RPN::operator=(const RPN &src) {
	if (this != &src)
		this->data = src.data;
	return (*this);
}

static bool is_ope(char c) {
	return (c == '+' || c == '-' || c == '/' || c == '*');
}

static bool is_digit(char c) {
	return (c >= '0' && c <= '9');
}

void	RPN::calculate(std::string ope) {
	for (size_t i = 0; i < ope.size(); i++)
	{
		char c = ope[i];

		if (c == ' ')
			continue ;
		else if (!is_digit(c) && !is_ope(c))
		{
			std::cerr << "Error: invalid argument: " << c << std::endl;
			return ;
		}
		else if (is_digit(c))
		{
			double number = c - '0';
			data.push(number);
		}
		else if (is_ope(c))
		{
			if (data.size() < 2)
			{
				std::cerr << "Error: missing numbers to execute operator '" << c << "'" << std::endl;
				return ;
			}
			double nb1 = data.top();
			data.pop();
			double nb2 = data.top();
			data.pop();
			if (c == '+')
				data.push(nb1 + nb2);
			else if (c == '-')
				data.push(nb2 - nb1);
			else if (c == '/')
			{
				if (nb1 == 0)
				{
					std::cerr << "Error: division by a null number is impossible." << std::endl;
					return ;
				}
				data.push(nb2 / nb1);
			}
			else
				data.push(nb2 * nb1);
		}
	}
	if (data.size() == 1)
	{
		double result = data.top();
		data.pop();
		std::cout << result << std::endl;
	}
	else
		std::cerr << "Error: Missing operators..." << std::endl;
}
