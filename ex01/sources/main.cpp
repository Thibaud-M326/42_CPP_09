#include "RPN.hpp"
#include <string>
#include <iostream>
#include <exception>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage: ./rpn <rpn expression>";
		return 1;
	}
	try {
		std::string expression = argv[1];
		Rpn rpn;

		rpn.evaluate(expression);
	}	catch (std::exception& e) {
		std::cout << "Error : " << e.what() << std::endl;
	}
}
