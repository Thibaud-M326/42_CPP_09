#include "PmergeMe.hpp"
#include <limits>
#include <iostream>
#include <vector>

void	PmergeMe::isValidArgs(std::string arg) 
{
	std::string::iterator it;
	int digitSize = 0;

	for (it = arg.begin(); it != arg.end(); ++it)
	{
		if (*it == ' ' && *(it + 1) == '-' && std::isdigit(*(it + 2)))
			throw std::runtime_error(std::string("only positive number allowed"));
		if (!std::isdigit(*it) && *it != ' ')
			throw std::runtime_error(std::string("bad arguments input"));
		if (std::isdigit(*it))
			digitSize++;
		if (*it == ' ')
			digitSize = 0;
		if (digitSize > 10)
			throw std::runtime_error(std::string("bad arguments input, int overflow"));
	}
}

std::vector<int> PmergeMe::parseInts(const std::string& str, std::vector<int>& values)
{
	std::istringstream iss(str);
	long n;

	while (iss >> n)
	{
		_elements++;
		if (n > std::numeric_limits<int>::max() || n < std::numeric_limits<int>::min())
			throw std::runtime_error(std::string("bad arguments input, int overflow"));
		if (_elements > 3000)
			throw std::runtime_error(std::string("3000 elements max to sort"));
		values.push_back(n);
	}
	return values;
}

void PmergeMe::printVector(const std::vector<int>& vec)
{
	std::vector<int>::const_iterator it;

	for (it = vec.begin(); it != vec.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}
