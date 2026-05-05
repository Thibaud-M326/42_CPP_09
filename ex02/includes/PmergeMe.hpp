#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <string>
#include <vector>
#include <sstream>

typedef std::pair<int, int> IntPair;
typedef std::vector<IntPair> PairVec;

class PmergeMe {
	private:
		PmergeMe(const PmergeMe& copy);
		PmergeMe& operator=(const PmergeMe& other);

		int _compairCount;

		void							isValidArgs(std::string unsortedInts);
		std::vector<int>	parseInts(const std::string& str);
		void							printVector(const std::vector<int>& vec);

		std::vector<int> 	pmerge(std::vector<int> toSort);
		void							makePair(std::vector<int>& toPair, PairVec& pair, int& unpaired);
		void							createMain(std::vector<int>& main, const PairVec& pair);
		std::vector<int> 	idxsJacobsthal(int size);
		int								idxJacobsthal(int n);
		int								binarySearch(std::vector<int> &arr, int high, int x);
		std::vector<int> 	sortNextMain(std::vector<int>& nextMain, PairVec& pend, int& unpaired, std::vector<int> idxsJacob);
		std::vector<int> 	completedReversedJacobsthal(std::vector<int> jacob);
		std::vector<int> 	getIdxsFromJacobsthal(std::vector<int> jacob);

	public:
		PmergeMe();
		~PmergeMe();

		void sort(std::string unsortedInts);
};

#endif
