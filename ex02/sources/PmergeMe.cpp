#include "PmergeMe.hpp"
#include <stdexcept>
#include <string>
#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>

PmergeMe::PmergeMe()
:
	_elements(0),
	_compairCount(0)
{}

PmergeMe::~PmergeMe()
{}

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
		_compairCount++;
	}
}

void PmergeMe::createMain(std::vector<int>& main, const PairVec& pair)
{
	PairVec::const_iterator it;

	for (it = pair.begin(); it != pair.end(); ++it)
		main.push_back(it->second);
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

std::vector<int> PmergeMe::sortNextMain(std::vector<int>& nextMain, PairVec& pend, int& unpaired)
{
	std::vector<int>::iterator jacobIt;

	for (jacobIt = _idxsJacob.begin(); jacobIt != _idxsJacob.end(); ++jacobIt)
	{
		if ((unsigned long)*jacobIt < pend.size())
		{
			PairVec::iterator pendIt = pend.begin() + *jacobIt;

			std::vector<int>::iterator bigIt = std::find(nextMain.begin(), nextMain.end(), pendIt->second);
			int bigValuePos = std::distance(nextMain.begin(), bigIt);
			int sortIndex = binarySearch(nextMain, bigValuePos, pendIt->first);

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

	nextMain = sortNextMain(nextMain, pend, unpaired);

	return nextMain;
}

void PmergeMe::sort(std::string arg)
{
	isValidArgs(arg);
	std::vector<int> vecValues;
	int jacobMaxSequence = 12;

	parseInts(arg, vecValues);
	_idxsJacob = idxsJacobsthal(jacobMaxSequence);

	std::cout << "before: ";
	printVector(vecValues);

	clock_t start = clock();
	std::vector<int> sorted = pmerge(vecValues);
	clock_t end = clock();

	std::cout << "after:  ";
	printVector(sorted);

  double elapsed_us = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;
	std::cout << "Time to process a range of " << _elements << " elements with std::vector : " << std::fixed << std::setprecision(5) << elapsed_us << "us" << std::endl;

	std::cout << "_compairCount:" << _compairCount << std::endl;
}
