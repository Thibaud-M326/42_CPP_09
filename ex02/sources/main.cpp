#include "PmergeMe.hpp"
#include <iostream>
#include <exception>

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "Usage: ./PmergeMe <positive integer, ...>" << std::endl;
		return 1;
	}
	try {
		PmergeMe pmerge;
		pmerge.sort(argc, argv);
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
