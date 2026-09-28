#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <utility>
#include <cstddef>

class PmergeMe {
	private:

		// An element is (value, id). The id is unique inside a recursion level and
		// links a "big" to its pair, so we never have to search a value again.
		typedef std::pair<int, int>                    Elem;
		// (small, big)
		typedef std::pair<Elem, Elem>                  ElemPair;

		typedef std::vector<int>                       IntVector;
		typedef std::vector<Elem>                      ElemVector;
		typedef std::vector<ElemPair>                  ElemPairVector;

		typedef std::deque<int>                        IntDeque;
		typedef std::deque<Elem>                       ElemDeque;
		typedef std::deque<ElemPair>                   ElemPairDeque;

		PmergeMe(const PmergeMe& copy);
		PmergeMe& operator=(const PmergeMe& other);

		int								_elements;

		void							isValidArgs(int argc, char** argv);

		// jacobsthal
		std::vector<int>	jacobsthalOrder(int size);

		// std::vector
		void							parseVector(int argc, char** argv, IntVector& values);
		void							printVector(const IntVector& values);
		IntVector					sortVector(const IntVector& values);
		ElemVector				pmergeVector(const ElemVector& toSort);
		void							makePairVector(const ElemVector& toPair, ElemPairVector& pairs, bool& hasUnpaired, Elem& unpaired);
		void							createMainVector(ElemVector& mains, const ElemPairVector& pairs);
		ElemVector				insertPendVector(const ElemVector& sortedMains, const ElemPairVector& pairs, bool hasUnpaired, const Elem& unpaired);
		size_t						binarySearchVector(const ElemVector& arr, size_t end, int x);
		size_t						findPosVector(const ElemVector& arr, int id);

		// std::deque
		void							parseDeque(int argc, char** argv, IntDeque& values);
		IntDeque					sortDeque(const IntDeque& values);
		ElemDeque					pmergeDeque(const ElemDeque& toSort);
		void							makePairDeque(const ElemDeque& toPair, ElemPairDeque& pairs, bool& hasUnpaired, Elem& unpaired);
		void							createMainDeque(ElemDeque& mains, const ElemPairDeque& pairs);
		ElemDeque					insertPendDeque(const ElemDeque& sortedMains, const ElemPairDeque& pairs, bool hasUnpaired, const Elem& unpaired);
		size_t						binarySearchDeque(const ElemDeque& arr, size_t end, int x);
		size_t						findPosDeque(const ElemDeque& arr, int id);

	public:
		PmergeMe();
		~PmergeMe();

		void sort(int argc, char** argv);
};

#endif
