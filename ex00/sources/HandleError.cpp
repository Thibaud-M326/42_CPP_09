#include "HandleError.hpp"

#include <exception>
#include <string>
#include <iostream>

void HandleError::handleError(std::string errMsg, std::string wrongInput)
{
	std::string errorMessage = "Error: " + errMsg + " => " + wrongInput;
	
	throw std::runtime_error(errorMessage);
}

