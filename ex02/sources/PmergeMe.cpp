#include "PmergeMe.hpp"
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <ctime>

PmergeMe::PmergeMe()
:
	_elements(0),
	_idxsJacob()
{}

PmergeMe::~PmergeMe()
{}

/* ------------------------------ parsing ------------------------------ */

void PmergeMe::isValidArgs(std::string arg)
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

void PmergeMe::parseVector(const std::string& str, IntVector& values)
{
	std::istringstream iss(str);
	long n;

	while (iss >> n)
	{
		if (n > std::numeric_limits<int>::max() || n < std::numeric_limits<int>::min())
			throw std::runtime_error(std::string("bad arguments input, int overflow"));
		if (values.size() >= 3000)
			throw std::runtime_error(std::string("3000 elements max to sort"));
		values.push_back(n);
	}
}

void PmergeMe::parseDeque(const std::string& str, IntDeque& values)
{
	std::istringstream iss(str);
	long n;

	while (iss >> n)
	{
		if (n > std::numeric_limits<int>::max() || n < std::numeric_limits<int>::min())
			throw std::runtime_error(std::string("bad arguments input, int overflow"));
		if (values.size() >= 3000)
			throw std::runtime_error(std::string("3000 elements max to sort"));
		values.push_back(n);
	}
}

void PmergeMe::printVector(const IntVector& values)
{
	IntVector::const_iterator it;

	for (it = values.begin(); it != values.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

void PmergeMe::printDeque(const IntDeque& values)
{
	IntDeque::const_iterator it;

	for (it = values.begin(); it != values.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

/* ----------------------------- jacobsthal ---------------------------- */

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
			completeJacob.push_back(current);
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
	for (int i = 3; i < size + 3; i++)
		jac.push_back(idxJacobsthal(i) - 2);

	return getIdxsFromJacobsthal(jac);
}

/* ------------------------------ std::vector -------------------------- */

void PmergeMe::makePairVector(IntVector& toPair, PairVector& pair, int& unpaired)
{
	if (toPair.size() % 2 != 0)
	{
		unpaired = toPair.back();
		toPair.pop_back();
	}

	IntVector::iterator it;

	for (it = toPair.begin(); it != toPair.end(); it += 2)
	{
		if (*it < *(it + 1))
			pair.push_back(std::make_pair(*it, *(it + 1)));
		else
			pair.push_back(std::make_pair(*(it + 1), *it));
	}
}

void PmergeMe::createMainVector(IntVector& main, const PairVector& pair)
{
	PairVector::const_iterator it;

	for (it = pair.begin(); it != pair.end(); ++it)
		main.push_back(it->second);
}

int PmergeMe::binarySearchVector(IntVector& arr, int high, int x)
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

PmergeMe::IntVector PmergeMe::sortNextMainVector(IntVector& nextMain, PairVector& pend, int& unpaired)
{
	std::vector<int>::iterator jacobIt;

	for (jacobIt = _idxsJacob.begin(); jacobIt != _idxsJacob.end(); ++jacobIt)
	{
		if ((unsigned long)*jacobIt < pend.size())
		{
			PairVector::iterator pendIt = pend.begin() + *jacobIt;

			IntVector::iterator bigIt = std::find(nextMain.begin(), nextMain.end(), pendIt->second);
			int bigValuePos = std::distance(nextMain.begin(), bigIt);
			int sortIndex = binarySearchVector(nextMain, bigValuePos, pendIt->first);

			IntVector::iterator sortIndexIt = nextMain.begin() + sortIndex;
			nextMain.insert(sortIndexIt, pendIt->first);
		}
	}

	if (unpaired != -1)
	{
		int sortIndexUnpaired = binarySearchVector(nextMain, nextMain.size() - 1, unpaired);
		IntVector::iterator sortIndexUnpairedIt = nextMain.begin() + sortIndexUnpaired;
		nextMain.insert(sortIndexUnpairedIt, unpaired);
	}
	return nextMain;
}

PmergeMe::IntVector PmergeMe::pmergeVector(IntVector toSort)
{
	if (toSort.size() < 2)
		return toSort;
	PairVector pair;
	int unpaired = -1;

	makePairVector(toSort, pair, unpaired);
	PairVector pend = pair;

	IntVector main;
	IntVector nextMain;
	createMainVector(main, pair);

	nextMain = pmergeVector(main);

	nextMain = sortNextMainVector(nextMain, pend, unpaired);

	return nextMain;
}

/* ------------------------------ std::deque --------------------------- */

void PmergeMe::makePairDeque(IntDeque& toPair, PairDeque& pair, int& unpaired)
{
	if (toPair.size() % 2 != 0)
	{
		unpaired = toPair.back();
		toPair.pop_back();
	}

	IntDeque::iterator it;

	for (it = toPair.begin(); it != toPair.end(); it += 2)
	{
		if (*it < *(it + 1))
			pair.push_back(std::make_pair(*it, *(it + 1)));
		else
			pair.push_back(std::make_pair(*(it + 1), *it));
	}
}

void PmergeMe::createMainDeque(IntDeque& main, const PairDeque& pair)
{
	PairDeque::const_iterator it;

	for (it = pair.begin(); it != pair.end(); ++it)
		main.push_back(it->second);
}

int PmergeMe::binarySearchDeque(IntDeque& arr, int high, int x)
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

PmergeMe::IntDeque PmergeMe::sortNextMainDeque(IntDeque& nextMain, PairDeque& pend, int& unpaired)
{
	std::vector<int>::iterator jacobIt;

	for (jacobIt = _idxsJacob.begin(); jacobIt != _idxsJacob.end(); ++jacobIt)
	{
		if ((unsigned long)*jacobIt < pend.size())
		{
			PairDeque::iterator pendIt = pend.begin() + *jacobIt;

			IntDeque::iterator bigIt = std::find(nextMain.begin(), nextMain.end(), pendIt->second);
			int bigValuePos = std::distance(nextMain.begin(), bigIt);
			int sortIndex = binarySearchDeque(nextMain, bigValuePos, pendIt->first);

			IntDeque::iterator sortIndexIt = nextMain.begin() + sortIndex;
			nextMain.insert(sortIndexIt, pendIt->first);
		}
	}

	if (unpaired != -1)
	{
		int sortIndexUnpaired = binarySearchDeque(nextMain, nextMain.size() - 1, unpaired);
		IntDeque::iterator sortIndexUnpairedIt = nextMain.begin() + sortIndexUnpaired;
		nextMain.insert(sortIndexUnpairedIt, unpaired);
	}
	return nextMain;
}

PmergeMe::IntDeque PmergeMe::pmergeDeque(IntDeque toSort)
{
	if (toSort.size() < 2)
		return toSort;
	PairDeque pair;
	int unpaired = -1;

	makePairDeque(toSort, pair, unpaired);
	PairDeque pend = pair;

	IntDeque main;
	IntDeque nextMain;
	createMainDeque(main, pair);

	nextMain = pmergeDeque(main);

	nextMain = sortNextMainDeque(nextMain, pend, unpaired);

	return nextMain;
}

/* -------------------------------- sort ------------------------------- */

void PmergeMe::sort(std::string arg)
{
	isValidArgs(arg);
	int jacobMaxSequence = 12;
	_idxsJacob = idxsJacobsthal(jacobMaxSequence);

	IntVector vec;
	parseVector(arg, vec);
	_elements = vec.size();

	std::cout << "before: ";
	printVector(vec);

	clock_t startVec = clock();
	IntVector sortedVec = pmergeVector(vec);
	clock_t endVec = clock();

	std::cout << "after:  ";
	printVector(sortedVec);

	IntDeque deq;
	parseDeque(arg, deq);

	clock_t startDeq = clock();
	IntDeque sortedDeq = pmergeDeque(deq);
	clock_t endDeq = clock();

	double elapsedVec = static_cast<double>(endVec - startVec) / CLOCKS_PER_SEC * 1e6;
	double elapsedDeq = static_cast<double>(endDeq - startDeq) / CLOCKS_PER_SEC * 1e6;

	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Time to process a range of " << _elements << " elements with std::vector : " << elapsedVec << "us" << std::endl;
	std::cout << "Time to process a range of " << _elements << " elements with std::deque : " << elapsedDeq << "us" << std::endl;
}
