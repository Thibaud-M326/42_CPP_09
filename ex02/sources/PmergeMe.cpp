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
	_compare(0)
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

	for (it = toPair.begin(); it != toPair.end(); it += 2)
	{
		if (*it < *(it + 1))
			pair.push_back(std::make_pair(*it, *(it + 1)));
		else
			pair.push_back(std::make_pair(*(it + 1), *it));
		std::cout << *it << " " << *(it + 1) << std::endl;
		_compare++;
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

std::vector<int> PmergeMe::idxsJacobsthal(size_t size)
{
	std::vector<int> idxs;

	for (size_t i = 4; i < size + 4; i++)
		idxs.push_back(idxJacobsthal(i - 2));
	return idxs;
}

int PmergeMe::binarySearch(std::vector<int>& v, int target, int high) 
{
	int low = 0;

	while (low <= high) 
	{
		_compare++;
		int mid = ((high - low) / 2) + low;

		if (v[mid] == target) {
			_compare++;
			return true;
		}
		if (v[mid] > target)
			high = mid - 1;
		else
			low = mid + 1;
		_compare++;
	}
	return low;
}

std::vector<int> PmergeMe::sortNextMain(std::vector<int>& nextMain, PairVec& pend, int& unpaired, std::vector<int> idxsJacob)
{
	std::vector<int>::iterator jacobIt;
	std::vector<bool> inserted(pend.size(), false);

	for (jacobIt = idxsJacob.begin(); jacobIt != idxsJacob.end(); ++jacobIt)
	{
		PairVec::iterator pendIt = pend.begin() + *jacobIt - 1;

		// std::vector<int>::iterator bigValueInNextMain = lower_bound(nextMain.begin(), nextMain.end(), pendIt->second);
		
		std::vector<int>::iterator HighIt = find(nextMain.begin(), nextMain.end(), pendIt->second);

		int sortIndex;
		if (HighIt == nextMain.end())
			sortIndex = binarySearch(nextMain, pendIt->first, nextMain.size() - 1);
		else
		{
			int highPos = std::distance(nextMain.begin(), HighIt);
			sortIndex = binarySearch(nextMain, pendIt->first, highPos);
		}

		std::vector<int>::iterator vecIt = nextMain.begin() + sortIndex;
		nextMain.insert(vecIt, pendIt->first);
		inserted[*jacobIt] = true;
	}

	if (unpaired != -1)
	{
		std::vector<int>::iterator sortIndexUnpaired = lower_bound(nextMain.begin(), nextMain.end(), unpaired);
		nextMain.insert(sortIndexUnpaired, unpaired);
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

	std::cout << "jacob:" << std::endl;
	printVector(idxsJacob);

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
	std::cout << "compare:" << _compare << std::endl; 
}
