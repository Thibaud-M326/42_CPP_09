#include <stdexcept>
#include <string>
#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <vector>

template <template <typename, typename> class Container>
PmergeMe<Container>::PmergeMe()
:
	_elements(0),
	_idxsJacob()
{}

template <template <typename, typename> class Container>
PmergeMe<Container>::~PmergeMe()
{}

template <template <typename, typename> class Container>
void PmergeMe<Container>::makePair(IntContainer& toPair, PairVec& pair, int& unpaired)
{
	if (toPair.size() % 2 != 0)
	{
		unpaired = toPair.back();
		toPair.pop_back();
	}

	typename IntContainer::iterator it;

	for (it = toPair.begin(); it != toPair.end(); it += 2)
	{
		if (*it < *(it + 1))
			pair.push_back(std::make_pair(*it, *(it + 1)));
		else
			pair.push_back(std::make_pair(*(it + 1), *it));
	}
}

template <template <typename, typename> class Container>
void PmergeMe<Container>::createMain(IntContainer& main, const PairVec& pair)
{
	typename PairVec::const_iterator it;

	for (it = pair.begin(); it != pair.end(); ++it)
		main.push_back(it->second);
}

template <template <typename, typename> class Container>
int PmergeMe<Container>::binarySearch(IntContainer &arr, int high, int x) 
{
	int low = 0;
	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (arr[mid] < x)
			low = mid + 1;
		else
			high = mid - 1;
	}
	return low;
}

template <template <typename, typename> class Container>
typename PmergeMe<Container>::IntContainer
PmergeMe<Container>::sortNextMain(IntContainer& nextMain, PairVec& pend, int& unpaired)
{
	typename IntContainer::iterator jacobIt;

	for (jacobIt = _idxsJacob.begin(); jacobIt != _idxsJacob.end(); ++jacobIt)
	{
		if ((unsigned long)*jacobIt < pend.size())
		{
			typename PairVec::iterator pendIt = pend.begin() + *jacobIt;

			typename IntContainer::iterator bigIt = std::find(nextMain.begin(), nextMain.end(), pendIt->second);
			int bigValuePos = std::distance(nextMain.begin(), bigIt);
			int sortIndex = binarySearch(nextMain, bigValuePos, pendIt->first);

			typename IntContainer::iterator sortIndexIt = nextMain.begin() + sortIndex;
			nextMain.insert(sortIndexIt, pendIt->first);
		}
	}

	if (unpaired != -1)
	{
		int sortIndexUnpaired = binarySearch(nextMain, nextMain.size() - 1, unpaired);
		typename IntContainer::iterator sortIndexUnpairedIt = nextMain.begin() + sortIndexUnpaired;
		nextMain.insert(sortIndexUnpairedIt, unpaired);
	}
	return nextMain;
}

template <template <typename, typename> class Container>
typename PmergeMe<Container>::IntContainer PmergeMe<Container>::pmerge(IntContainer toSort)
{
	if (toSort.size() < 2)
		return toSort;
	PairVec pair;
	int unpaired = -1;

	makePair(toSort, pair, unpaired);
	PairVec pend = pair;

	IntContainer main;
	IntContainer nextMain;
	createMain(main, pair);
	
	nextMain = pmerge(main);

	nextMain = sortNextMain(nextMain, pend, unpaired);

	return nextMain;
}

template <template <typename, typename> class Container>
void PmergeMe<Container>::sort(std::string arg)
{
	isValidArgs(arg);
	IntContainer values;
	int jacobMaxSequence = 12;

	parseInts(arg, values);
	_idxsJacob = idxsJacobsthal(jacobMaxSequence);

	std::cout << "before: ";
	printContainer(values);

	clock_t start = clock();
	IntContainer sorted = pmerge(values);
	clock_t end = clock();

	std::cout << "after:  ";
	printContainer(sorted);

  double elapsed_us = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;
	std::cout << "Time to process a range of " << _elements << " elements with std::Container : " << std::fixed << std::setprecision(5) << elapsed_us << "us" << std::endl;
}
