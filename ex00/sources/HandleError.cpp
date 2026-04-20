#include "HandleError.hpp"

#include <exception>
#include <string>
#include <iostream>

void HandleError::handleError(std::string errMsg, std::string wrongInput, int throwError)
{
	std::string errorMessage = "Error: " + errMsg + " => " + wrongInput;

	if (throwError)
		throw std::runtime_error(errorMessage);
	std::cout << errorMessage << std::endl;
}

