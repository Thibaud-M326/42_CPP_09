#include "PmergeMe.hpp"
#include <iostream>

std::vector<int> PmergeMe::getIdxsFromJacobsthal(std::vector<int> jacob)
{
	std::vector<int> completeJacob;
	int previous = -1;
	int current = 0;

	std::vector<int>::iterator it;
	for (it = jacob.begin(); it != jacob.end(); ++it)
	{
		current = *it;
		for (; current > previous; current--)
		{
			completeJacob.push_back(current);
		}
		previous = *it;
	}
	return completeJacob;
}

int PmergeMe::idxJacobsthal(int n)
{
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	return idxJacobsthal(n - 1) + 2 * idxJacobsthal(n - 2);
}

std::vector<int> PmergeMe::idxsJacobsthal(int size)
{
	std::vector<int> jac;
	std::vector<int> idxs;

	for (int i = 3; i < size + 3; i++)
		jac.push_back(idxJacobsthal(i) - 2);

	idxs = getIdxsFromJacobsthal(jac);

	return idxs;
}

