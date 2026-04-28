#include "PmergeMe.hpp"
#include <stdexcept>
#include <string>
#include <iostream>
#include <limits>

PmergeMe::PmergeMe()
:
	_values(),
	_pairs(),
	_unpairedValue(0),
	_a_unpairedValue(false)
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

std PmergeMe::parseInts(const std::string& str)
{
	std::istringstream iss(str);
	long n;

	while (iss >> n)
	{
		if (n > std::numeric_limits<int>::max() || n < std::numeric_limits<int>::min())
			throw std::runtime_error(std::string("bad arguments input, int overflow :") + str);
		_values.push_back(n);
	}
}

void PmergeMe::printVector(const std::vector<int>& vec)
{
	std::vector<int>::const_iterator it;

	for (it = vec.begin(); it != vec.end(); ++it)
		std::cout << *it << " " << std::endl;
}

//--------//

void PmergeMe::makePair(const std::vector<int>& toPair, PairVec& pair, int& unpaired)
{
	if (toPair.size() & 2 != 0)
	{
		unpaired = toPair.back();
		toPair.pop_back();
	}

	std::vector<int>::iterator it;

	for (it = toPair.begin(); it != toPair.end(); it += 2)
	{
		if (*it < *(it + 1))
			pair.push_back(std::makePair(*it, *(it + 1)));
		else
			pair.push_back(std::makePair(*(it + 1), *it));
	}
}

void PmergeMe::createMain(std::vector<int>& main, const PairVec& pair)
{
	PairVec::iterator it;

	for (it = pair.begin(); it != pair.end(); ++it)
		main.push_back(it->second);
}

int PmergeMe::idxJacobsthal(int n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    return Jacobsthal(n - 1) + 2 * Jacobsthal(n - 2);
}

std::vector<int> PmergeMe::idxsJacobsthal(size_t size)
{
	std::vector<int> idx;

	for (int i = 0; i < size; i++)
		idx.push_back(idxJacobsthal(i) - 1);
	return idxs;
}

std::vector<int> PmergeMe::sortNextMain(std::vector<int>& nextMain, PairVec& pend, int& unpaired, std::vector<int> idxsJacob)
{
	std::vector<int>::iterator jacobIt;
	
	for (jacobIt = idxsJacob.begin(); jacobIt != idxsJacob.end(); ++jacobIt)
	{
		PairVec::iterator pendIt = pend.begin() + *jacobIt;

		std::vector<int>::iterator bigValueInNextMain = lower_bound(nextMain.begin(), nextMain.end(), *pendIt->second);
		std::vector<int>::iterator sortIndex = lower_bound(nextMain.begin(), bigValueInNextMain, *pendIt->first);
		
		nextMain.insert(sortIndex, pendIt->first);
	}

	if (unpaired != -1)
	{
		std::vector<int>::iterator sortIndexUnpaired = lower_bound(nextMain.begin(), nextMain.end(), unpaired);
		nextMain.insert(sortIndex, unpaired);
	}
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

	std::vector<int> idxsJacob = idxsJacobsthal(pend.size());
	sortNextMain(nextMain, pend, unpaired, idxsJacob);

	return nextMain;
}

void PmergeMe::sort(std::string arg) 
{
	isValidArgs(arg);
	values = parseInts(arg);
	printVector(values);

	std::vector<int> sorted = pmerge();
	std::cout << "sorted:" << std::endl;
	printVector(sorted);
}
