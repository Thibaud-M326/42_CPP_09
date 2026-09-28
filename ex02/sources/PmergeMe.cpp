#include "PmergeMe.hpp"
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime>

PmergeMe::PmergeMe()
:
	_elements(0)
{}

PmergeMe::~PmergeMe()
{}

/* ------------------------------ parsing ------------------------------ */

void PmergeMe::isValidArgs(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];

		if (arg.empty())
			throw std::runtime_error(std::string("empty argument"));

		for (std::string::iterator it = arg.begin(); it != arg.end(); ++it)
			if (!std::isdigit(static_cast<unsigned char>(*it)))
				throw std::runtime_error(std::string("only positive integers allowed : ") + arg);

		if (arg.size() > 10)
			throw std::runtime_error(std::string("int overflow : ") + arg);

		std::istringstream iss(arg);
		unsigned long n;
		iss >> n;
		if (iss.fail() || n > static_cast<unsigned long>(std::numeric_limits<int>::max()))
			throw std::runtime_error(std::string("int overflow : ") + arg);
	}
}

void PmergeMe::parseVector(int argc, char** argv, IntVector& values)
{
	for (int i = 1; i < argc; i++)
		values.push_back(std::atoi(argv[i]));
}

void PmergeMe::parseDeque(int argc, char** argv, IntDeque& values)
{
	for (int i = 1; i < argc; i++)
		values.push_back(std::atoi(argv[i]));
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

// Insertion order of the "pend" elements (0-based) : b1, b3 b2, b5 b4, b11..b6, ...
// Each group ends on a Jacobsthal number (1, 3, 5, 11, 21, ...), capped to size.
std::vector<int> PmergeMe::jacobsthalOrder(int size)
{
	std::vector<int> order;

	if (size <= 0)
		return order;
	order.push_back(0);

	int jPrev = 1;
	int jCurr = 3;
	while (jPrev < size)
	{
		int top = std::min(jCurr, size);
		for (int i = top; i > jPrev; i--)
			order.push_back(i - 1);

		int jNext = jCurr + 2 * jPrev;
		jPrev = jCurr;
		jCurr = jNext;
	}
	return order;
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
	std::vector<int> order = jacobsthalOrder(pend.size());
	std::vector<int>::iterator orderIt;

	for (orderIt = order.begin(); orderIt != order.end(); ++orderIt)
	{
		PairVector::iterator pendIt = pend.begin() + *orderIt;

		IntVector::iterator bigIt = std::find(nextMain.begin(), nextMain.end(), pendIt->second);
		int bigValuePos = std::distance(nextMain.begin(), bigIt);
		int sortIndex = binarySearchVector(nextMain, bigValuePos, pendIt->first);

		IntVector::iterator sortIndexIt = nextMain.begin() + sortIndex;
		nextMain.insert(sortIndexIt, pendIt->first);
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
	std::vector<int> order = jacobsthalOrder(pend.size());
	std::vector<int>::iterator orderIt;

	for (orderIt = order.begin(); orderIt != order.end(); ++orderIt)
	{
		PairDeque::iterator pendIt = pend.begin() + *orderIt;

		IntDeque::iterator bigIt = std::find(nextMain.begin(), nextMain.end(), pendIt->second);
		int bigValuePos = std::distance(nextMain.begin(), bigIt);
		int sortIndex = binarySearchDeque(nextMain, bigValuePos, pendIt->first);

		IntDeque::iterator sortIndexIt = nextMain.begin() + sortIndex;
		nextMain.insert(sortIndexIt, pendIt->first);
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

void PmergeMe::sort(int argc, char** argv)
{
	isValidArgs(argc, argv);

	IntVector vec;
	parseVector(argc, argv, vec);
	_elements = vec.size();

	std::cout << "before: ";
	printVector(vec);

	clock_t startVec = clock();
	IntVector sortedVec = pmergeVector(vec);
	clock_t endVec = clock();

	std::cout << "after:  ";
	printVector(sortedVec);

	IntDeque deq;
	parseDeque(argc, argv, deq);

	clock_t startDeq = clock();
	IntDeque sortedDeq = pmergeDeque(deq);
	clock_t endDeq = clock();

	double elapsedVec = static_cast<double>(endVec - startVec) / CLOCKS_PER_SEC * 1e6;
	double elapsedDeq = static_cast<double>(endDeq - startDeq) / CLOCKS_PER_SEC * 1e6;

	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Time to process a range of " << _elements << " elements with std::vector : " << elapsedVec << "us" << std::endl;
	std::cout << "Time to process a range of " << _elements << " elements with std::deque : " << elapsedDeq << "us" << std::endl;
}
