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

    std::vector<int> _values;
    PairVec _pairs;
		int _unpairedValue;
		bool _a_unpairedValue;

		void	isValidArgs(std::string unsortedInts);
    void	parseInts(const std::string& str);
		void	printValues();
		void	makePair();
		void	pmerge();

	public:
		PmergeMe();
		~PmergeMe();

		void sort(std::string unsortedInts);
};

#endif
