#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <string>
#include <vector>
#include <deque>
#include <utility>

class PmergeMe {
	private:

		typedef std::vector<int>                       IntVector;
		typedef std::vector<std::pair<int, int> >      PairVector;
		typedef std::deque<int>                        IntDeque;
		typedef std::deque<std::pair<int, int> >       PairDeque;

		PmergeMe(const PmergeMe& copy);
		PmergeMe& operator=(const PmergeMe& other);

		int								_elements;
		std::vector<int>	_idxsJacob;

		void							isValidArgs(int argc, char** argv);

		// jacobsthal
		std::vector<int>	idxsJacobsthal(int size);
		int								idxJacobsthal(int n);
		std::vector<int>	getIdxsFromJacobsthal(std::vector<int> jacob);

		// std::vector
		void							parseVector(int argc, char** argv, IntVector& values);
		void							printVector(const IntVector& values);
		IntVector					pmergeVector(IntVector toSort);
		void							makePairVector(IntVector& toPair, PairVector& pair, int& unpaired);
		void							createMainVector(IntVector& main, const PairVector& pair);
		int								binarySearchVector(IntVector& arr, int high, int x);
		IntVector					sortNextMainVector(IntVector& nextMain, PairVector& pend, int& unpaired);

		// std::deque
		void							parseDeque(int argc, char** argv, IntDeque& values);
		void							printDeque(const IntDeque& values);
		IntDeque					pmergeDeque(IntDeque toSort);
		void							makePairDeque(IntDeque& toPair, PairDeque& pair, int& unpaired);
		void							createMainDeque(IntDeque& main, const PairDeque& pair);
		int								binarySearchDeque(IntDeque& arr, int high, int x);
		IntDeque					sortNextMainDeque(IntDeque& nextMain, PairDeque& pend, int& unpaired);

	public:
		PmergeMe();
		~PmergeMe();

		void sort(int argc, char** argv);
};

#endif
