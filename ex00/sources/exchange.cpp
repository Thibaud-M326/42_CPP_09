#include "BitcoinExchange.hpp"
#include "Parser.hpp"
#include "HandleError.hpp"
#include <string>
#include <iostream>

void	validateInputFileHeader(std::fstream& inputFile)
{
	if (!inputFile.is_open())
		throw std::runtime_error("can't read data.csv");

	std::string line;
	double throwError = false;

	std::getline(inputFile, line);
	if (line != "date | value")
		HandleError::handleError("invalid header format, should be : \"date | value\"", line, throwError);
}

void validateFormatInputFile(std::string line, bool throwError)
{
	std::size_t found = line.find(" | ", 10);
	if (found == std::string::npos)
		HandleError::handleError("bad input", line, throwError);
}

void BitcoinExchange::exchange(std::string inputFilename)
{
	std::fstream inputFile(inputFilename.c_str());
	if (!inputFile)
		throw std::runtime_error(std::string("can't open file : ") + inputFilename);

	std::string line;
	bool throwError = false;
	// double btcCount = 0;

	validateInputFileHeader(inputFile);

	while (std::getline(inputFile, line) && !inputFile.eof())
	{
		validateFormatInputFile(line, throwError);

		std::string parseDate = line.substr(0, 10);
		std::string parseValue = line.substr(13, line.size());

		Parser::validateDate(parseDate, throwError);

		std::cout << "parseDate :" << parseDate << std::endl;

		// btcCount = Parser::validateValue(parseValue, btcCount, throwError);
		//
		// if (btcCount < 0)
		// 	HandleError::handleError("not a positive number", line, throwError);
	}
}
