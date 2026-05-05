#include "PmergeMe.hpp"
#include <string>
#include <iostream>
#include <exception>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage: ./PmergeMe <int, ...>" << std::endl;
		return 1;
	}
	try {
		std::string unsortedInts = argv[1];
		PmergeMe pmerge;

		pmerge.sort(unsortedInts);
	}	catch (std::exception& e) {
		std::cout << "Error : " << e.what() << std::endl;
	}
}
