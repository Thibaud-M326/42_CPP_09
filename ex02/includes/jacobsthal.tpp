#include <iostream>

template <template <typename, typename> class Container>
typename PmergeMe<Container>::IntContainer PmergeMe<Container>::getIdxsFromJacobsthal(IntContainer jacob)
{
	IntContainer completeJacob;
	int previous = -1;
	int current = 0;

	typename IntContainer::iterator it;
	for (it = jacob.begin(); it != jacob.end(); ++it)
	{
		current = *it;
		for (; current > previous; current--)
			completeJacob.push_back(current);
		previous = *it;
	}
	return completeJacob;
}

template <template <typename, typename> class Container>
int PmergeMe<Container>::idxJacobsthal(int n)
{
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	return idxJacobsthal(n - 1) + 2 * idxJacobsthal(n - 2);
}

template <template <typename, typename> class Container>
typename PmergeMe<Container>::IntContainer PmergeMe<Container>::idxsJacobsthal(int size)
{
	IntContainer jac;
	for (int i = 3; i < size + 3; i++)
		jac.push_back(idxJacobsthal(i) - 2);

	IntContainer idxs;
	idxs = getIdxsFromJacobsthal(jac);
	return idxs;
}

