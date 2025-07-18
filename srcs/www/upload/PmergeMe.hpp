/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:11:34 by kbaridon          #+#    #+#             */
/*   Updated: 2025/06/05 12:45:40 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <list>

class PmergeMe
{
	private:
		int	ac;
		std::vector<int> vector;
		std::list<int> list;
		PmergeMe();
	public:
		PmergeMe(int ac, char **av);
		PmergeMe(const PmergeMe &copy);
		PmergeMe &operator=(const PmergeMe &src);
		~PmergeMe();
		void	Sort();
};

#endif