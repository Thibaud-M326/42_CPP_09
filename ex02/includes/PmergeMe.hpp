#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <string>
#include <sstream>

template <template <typename, typename> class Container>
class PmergeMe {
	private:

		typedef std::pair<int, int>                          IntPair;
		typedef Container<IntPair, std::allocator<IntPair> > PairVec;
		typedef Container<int, std::allocator<int> >         IntContainer;

		PmergeMe(const PmergeMe& copy);
		PmergeMe& operator=(const PmergeMe& other);

		int 							_elements;
		IntContainer			_idxsJacob;

		void							isValidArgs(std::string unsortedInts);
		IntContainer			parseInts(const std::string& str, IntContainer& values);

		void							printContainer(const IntContainer& vec);
		std::string				getContainerTypename(IntContainer* container);

		IntContainer 			pmerge(IntContainer toSort);
		void							makePair(IntContainer& toPair, PairVec& pair, int& unpaired);
		void							createMain(IntContainer& main, const PairVec& pair);
		IntContainer			idxsJacobsthal(int size);
		int								idxJacobsthal(int n);
		IntContainer			getIdxsFromJacobsthal(IntContainer jacob);
		int								binarySearch(IntContainer &arr, int high, int x);
		IntContainer			sortNextMain(IntContainer& nextMain, PairVec& pend, int& unpaired);

	public:
		PmergeMe();
		~PmergeMe();

		void sort(std::string unsortedInts);
};

#include "PmergeMe.tpp"
#include "jacobsthal.tpp"
#include "parsing.tpp"

#endif
