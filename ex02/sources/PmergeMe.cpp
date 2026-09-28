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

// pairs the elements as (small, big). The last one is kept apart if the count is odd.
void PmergeMe::makePairVector(const ElemVector& toPair, ElemPairVector& pairs, bool& hasUnpaired, Elem& unpaired)
{
	hasUnpaired = (toPair.size() % 2 != 0);
	if (hasUnpaired)
		unpaired = toPair.back();

	size_t pairedSize = toPair.size() - (toPair.size() % 2);
	for (size_t i = 0; i < pairedSize; i += 2)
	{
		if (toPair[i].first < toPair[i + 1].first)
			pairs.push_back(std::make_pair(toPair[i], toPair[i + 1]));
		else
			pairs.push_back(std::make_pair(toPair[i + 1], toPair[i]));
	}
}

// the bigs, each one labeled with the index of its pair
void PmergeMe::createMainVector(ElemVector& mains, const ElemPairVector& pairs)
{
	for (size_t i = 0; i < pairs.size(); i++)
		mains.push_back(Elem(pairs[i].second.first, static_cast<int>(i)));
}

// first position in [0, end) where x can be inserted
size_t PmergeMe::binarySearchVector(const ElemVector& arr, size_t end, int x)
{
	size_t low = 0;
	size_t high = end;

	while (low < high)
	{
		size_t mid = low + (high - low) / 2;
		if (arr[mid].first < x)
			low = mid + 1;
		else
			high = mid;
	}
	return low;
}

size_t PmergeMe::findPosVector(const ElemVector& arr, int id)
{
	size_t pos = 0;

	while (arr[pos].second != id)
		pos++;
	return pos;
}

PmergeMe::ElemVector PmergeMe::insertPendVector(const ElemVector& sortedMains, const ElemPairVector& pairs, bool hasUnpaired, const Elem& unpaired)
{
	ElemVector chain;
	ElemVector pend;

	// bigs in sorted order, and the smalls that follow them : chain = a1..ak, pend = b1..bk
	for (size_t i = 0; i < sortedMains.size(); i++)
	{
		const ElemPair& pair = pairs[sortedMains[i].second];
		chain.push_back(pair.second);
		pend.push_back(pair.first);
	}
	// the unpaired element is the last b, without any a
	if (hasUnpaired)
		pend.push_back(unpaired);

	std::vector<int> order = jacobsthalOrder(pend.size());
	std::vector<int>::iterator orderIt;

	for (orderIt = order.begin(); orderIt != order.end(); ++orderIt)
	{
		size_t idx = *orderIt;
		size_t end = chain.size();

		// search only before its big
		if (idx < sortedMains.size())
			end = findPosVector(chain, pairs[sortedMains[idx].second].second.second);

		size_t pos = binarySearchVector(chain, end, pend[idx].first);
		chain.insert(chain.begin() + pos, pend[idx]);
	}
	return chain;
}

PmergeMe::ElemVector PmergeMe::pmergeVector(const ElemVector& toSort)
{
	if (toSort.size() < 2)
		return toSort;

	ElemPairVector pairs;
	bool hasUnpaired = false;
	Elem unpaired;

	makePairVector(toSort, pairs, hasUnpaired, unpaired);

	ElemVector mains;
	createMainVector(mains, pairs);

	ElemVector sortedMains = pmergeVector(mains);

	return insertPendVector(sortedMains, pairs, hasUnpaired, unpaired);
}

PmergeMe::IntVector PmergeMe::sortVector(const IntVector& values)
{
	ElemVector elems;
	for (size_t i = 0; i < values.size(); i++)
		elems.push_back(Elem(values[i], static_cast<int>(i)));

	ElemVector sortedElems = pmergeVector(elems);

	IntVector sorted;
	for (size_t i = 0; i < sortedElems.size(); i++)
		sorted.push_back(sortedElems[i].first);
	return sorted;
}

/* ------------------------------ std::deque --------------------------- */

// pairs the elements as (small, big). The last one is kept apart if the count is odd.
void PmergeMe::makePairDeque(const ElemDeque& toPair, ElemPairDeque& pairs, bool& hasUnpaired, Elem& unpaired)
{
	hasUnpaired = (toPair.size() % 2 != 0);
	if (hasUnpaired)
		unpaired = toPair.back();

	size_t pairedSize = toPair.size() - (toPair.size() % 2);
	for (size_t i = 0; i < pairedSize; i += 2)
	{
		if (toPair[i].first < toPair[i + 1].first)
			pairs.push_back(std::make_pair(toPair[i], toPair[i + 1]));
		else
			pairs.push_back(std::make_pair(toPair[i + 1], toPair[i]));
	}
}

// the bigs, each one labeled with the index of its pair
void PmergeMe::createMainDeque(ElemDeque& mains, const ElemPairDeque& pairs)
{
	for (size_t i = 0; i < pairs.size(); i++)
		mains.push_back(Elem(pairs[i].second.first, static_cast<int>(i)));
}

// first position in [0, end) where x can be inserted
size_t PmergeMe::binarySearchDeque(const ElemDeque& arr, size_t end, int x)
{
	size_t low = 0;
	size_t high = end;

	while (low < high)
	{
		size_t mid = low + (high - low) / 2;
		if (arr[mid].first < x)
			low = mid + 1;
		else
			high = mid;
	}
	return low;
}

size_t PmergeMe::findPosDeque(const ElemDeque& arr, int id)
{
	size_t pos = 0;

	while (arr[pos].second != id)
		pos++;
	return pos;
}

PmergeMe::ElemDeque PmergeMe::insertPendDeque(const ElemDeque& sortedMains, const ElemPairDeque& pairs, bool hasUnpaired, const Elem& unpaired)
{
	ElemDeque chain;
	ElemDeque pend;

	// bigs in sorted order, and the smalls that follow them : chain = a1..ak, pend = b1..bk
	for (size_t i = 0; i < sortedMains.size(); i++)
	{
		const ElemPair& pair = pairs[sortedMains[i].second];
		chain.push_back(pair.second);
		pend.push_back(pair.first);
	}
	// the unpaired element is the last b, without any a
	if (hasUnpaired)
		pend.push_back(unpaired);

	std::vector<int> order = jacobsthalOrder(pend.size());
	std::vector<int>::iterator orderIt;

	for (orderIt = order.begin(); orderIt != order.end(); ++orderIt)
	{
		size_t idx = *orderIt;
		size_t end = chain.size();

		// search only before its big
		if (idx < sortedMains.size())
			end = findPosDeque(chain, pairs[sortedMains[idx].second].second.second);

		size_t pos = binarySearchDeque(chain, end, pend[idx].first);
		chain.insert(chain.begin() + pos, pend[idx]);
	}
	return chain;
}

PmergeMe::ElemDeque PmergeMe::pmergeDeque(const ElemDeque& toSort)
{
	if (toSort.size() < 2)
		return toSort;

	ElemPairDeque pairs;
	bool hasUnpaired = false;
	Elem unpaired;

	makePairDeque(toSort, pairs, hasUnpaired, unpaired);

	ElemDeque mains;
	createMainDeque(mains, pairs);

	ElemDeque sortedMains = pmergeDeque(mains);

	return insertPendDeque(sortedMains, pairs, hasUnpaired, unpaired);
}

PmergeMe::IntDeque PmergeMe::sortDeque(const IntDeque& values)
{
	ElemDeque elems;
	for (size_t i = 0; i < values.size(); i++)
		elems.push_back(Elem(values[i], static_cast<int>(i)));

	ElemDeque sortedElems = pmergeDeque(elems);

	IntDeque sorted;
	for (size_t i = 0; i < sortedElems.size(); i++)
		sorted.push_back(sortedElems[i].first);
	return sorted;
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
	IntVector sortedVec = sortVector(vec);
	clock_t endVec = clock();

	std::cout << "after:  ";
	printVector(sortedVec);

	IntDeque deq;
	parseDeque(argc, argv, deq);

	clock_t startDeq = clock();
	IntDeque sortedDeq = sortDeque(deq);
	clock_t endDeq = clock();

	double elapsedVec = static_cast<double>(endVec - startVec) / CLOCKS_PER_SEC * 1e6;
	double elapsedDeq = static_cast<double>(endDeq - startDeq) / CLOCKS_PER_SEC * 1e6;

	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Time to process a range of " << _elements << " elements with std::vector : " << elapsedVec << "us" << std::endl;
	std::cout << "Time to process a range of " << _elements << " elements with std::deque : " << elapsedDeq << "us" << std::endl;
}
