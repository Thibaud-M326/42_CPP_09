#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <string>
#include <vector>
#include <sstream>

template <typename Container>
class PmergeMe {
	private:

		typedef std::pair<int, int> IntPair;
		typedef Container <IntPair> PairVec;

		PmergeMe(const PmergeMe& copy);
		PmergeMe& operator=(const PmergeMe& other);

		int 			_elements;
		int 			_compairCount;
		Container	_idxsJacob;

		void							isValidArgs(std::string unsortedInts);
		Container					parseInts(const std::string& str, Container& values);
		void							printVector(const Container& vec);

		Container 				pmerge(Container toSort);
		void							makePair(Container& toPair, PairVec& pair, int& unpaired);
		void							createMain(Container& main, const PairVec& pair);
		Container					idxsJacobsthal(int size);
		int								idxJacobsthal(int n);
		Container					getIdxsFromJacobsthal(Container jacob);
		int								binarySearch(Container &arr, int high, int x);
		Container					sortNextMain(Container& nextMain, PairVec& pend, int& unpaired);

	public:
		PmergeMe();
		~PmergeMe();

		void sort(std::string unsortedInts);
};

#endif
