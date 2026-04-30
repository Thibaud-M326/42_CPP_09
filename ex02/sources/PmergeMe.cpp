#include "PmergeMe.hpp"
#include <stdexcept>
#include <string>
#include <iostream>
#include <limits>
#include <algorithm>

PmergeMe::PmergeMe()
:
	_values(),
	_pairs(),
	_unpairedValue(0),
	_a_unpairedValue(false),
	_compairCount(0)
{}

PmergeMe::~PmergeMe()
{}

void	PmergeMe::isValidArgs(std::string arg) 
{
	std::string::iterator it;
	int digitSize = 0;

	for (it = arg.begin(); it != arg.end(); ++it)
	{
		if (*it == ' ' && *(it + 1) == '-' && std::isdigit(*(it + 2)))
			throw std::runtime_error(std::string("only positive number allowed :") + arg);
		if (!std::isdigit(*it) && *it != ' ')
			throw std::runtime_error(std::string("bad arguments input :") + arg);
		if (std::isdigit(*it))
			digitSize++;
		if (*it == ' ')
			digitSize = 0;
		if (digitSize > 10)
			throw std::runtime_error(std::string("bad arguments input, int overflow :") + arg);
	}
}

std::vector<int> PmergeMe::parseInts(const std::string& str)
{
	std::istringstream iss(str);
	std::vector<int> values;
	long n;

	while (iss >> n)
	{
		if (n > std::numeric_limits<int>::max() || n < std::numeric_limits<int>::min())
			throw std::runtime_error(std::string("bad arguments input, int overflow :") + str);
		values.push_back(n);
	}
	return values;
}

void PmergeMe::printVector(const std::vector<int>& vec)
{
	std::vector<int>::const_iterator it;

	for (it = vec.begin(); it != vec.end(); ++it)
		std::cout << *it << " " << std::endl;
}

//--------//

void PmergeMe::makePair(std::vector<int>& toPair, PairVec& pair, int& unpaired)
{
	if (toPair.size() % 2 != 0)
	{
		unpaired = toPair.back();
		toPair.pop_back();
	}

	std::vector<int>::iterator it;

	std::cout << "toPair.size(): " << toPair.size() << std::endl;
	for (it = toPair.begin(); it != toPair.end(); it += 2)
	{
		if (*it < *(it + 1))
			pair.push_back(std::make_pair(*it, *(it + 1)));
		else
			pair.push_back(std::make_pair(*(it + 1), *it));
		_compairCount++;
	}
}

void PmergeMe::createMain(std::vector<int>& main, const PairVec& pair)
{
	PairVec::const_iterator it;

	for (it = pair.begin(); it != pair.end(); ++it)
		main.push_back(it->second);
}

int PmergeMe::idxJacobsthal(int n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    return idxJacobsthal(n - 1) + 2 * idxJacobsthal(n - 2);
}

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

std::vector<int> PmergeMe::idxsJacobsthal(int size)
{
	std::vector<int> jac;
	std::vector<int> idxs;

	for (int i = 3; i < size + 3; i++)
		jac.push_back(idxJacobsthal(i) - 2);

	idxs = getIdxsFromJacobsthal(jac);

	std::cout << "idxs:" << std::endl;
	printVector(idxs);
	
	return idxs;
}

int PmergeMe::binarySearch(std::vector<int> &arr, int high, int x) 
{
	int low = 0;
	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (arr[mid] < x)
			low = mid + 1;
		else
			high = mid - 1;
		_compairCount++;
	}
	return low;
}

std::vector<int> PmergeMe::sortNextMain(std::vector<int>& nextMain, PairVec& pend, int& unpaired, std::vector<int> idxsJacob)
{
	std::vector<int>::iterator jacobIt;

	for (jacobIt = idxsJacob.begin(); jacobIt != idxsJacob.end(); ++jacobIt)
	{
		if ((unsigned long)*jacobIt < pend.size())
		{
			PairVec::iterator pendIt = pend.begin() + *jacobIt;

			std::cout << "pendIt->second:" << pendIt->second << std::endl;

			std::vector<int>::iterator bigIt = std::find(nextMain.begin(), nextMain.end(), pendIt->second);
			int bigValuePos = std::distance(nextMain.begin(), bigIt);
			int sortIndex = binarySearch(nextMain, bigValuePos, pendIt->first);

			std::cout << "sortIndex:" << sortIndex << std::endl;

			// (void)sortIndex;
			std::vector<int>::iterator sortIndexIt = nextMain.begin() + sortIndex;
			nextMain.insert(sortIndexIt, pendIt->first);
		}
	}

	if (unpaired != -1)
	{
		int sortIndexUnpaired = binarySearch(nextMain, nextMain.size() - 1, unpaired);
		std::vector<int>::iterator sortIndexUnpairedIt = nextMain.begin() + sortIndexUnpaired;
		nextMain.insert(sortIndexUnpairedIt, unpaired);
	}
	return nextMain;
}

std::vector<int> PmergeMe::pmerge(std::vector<int> toSort)
{
	if (toSort.size() < 2)
		return toSort;
	PairVec pair;
	int unpaired = -1;

	makePair(toSort, pair, unpaired);
	PairVec pend = pair;

	std::vector<int> main;
	std::vector<int> nextMain;
	createMain(main, pair);
	
	nextMain = pmerge(main);

	std::cout << "pend.size():" << pend.size() << std::endl;
	std::vector<int> idxsJacob = idxsJacobsthal(pend.size());

	nextMain = sortNextMain(nextMain, pend, unpaired, idxsJacob);

	return nextMain;
}

void PmergeMe::sort(std::string arg) 
{
	isValidArgs(arg);
	std::vector<int> values = parseInts(arg);
	std::cout << "unsorted:" << std::endl;
	printVector(values);

	std::vector<int> sorted = pmerge(values);
	std::cout << "sorted:" << std::endl;
	printVector(sorted);

	std::cout << "_compairCount:" << _compairCount << std::endl;
}
