#include <limits>
#include <iostream>
#include <vector>

template <template <typename, typename> class Container>
void PmergeMe<Container>::isValidArgs(std::string arg) 
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

template <template <typename, typename> class Container>
typename PmergeMe<Container>::IntContainer PmergeMe<Container>::parseInts(const std::string& str, IntContainer& values)
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

template <template <typename, typename> class Container>
void PmergeMe<Container>::printContainer(const IntContainer& container)
{
	typename IntContainer::const_iterator it;

	for (it = container.begin(); it != container.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}
