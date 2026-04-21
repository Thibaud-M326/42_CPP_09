#include "BitcoinExchange.hpp"
#include "Parser.hpp"
#include "HandleError.hpp"
#include <string>
#include <iostream>
#include <exception>

void	validateInputFileHeader(std::fstream& inputFile)
{
	if (!inputFile.is_open())
		throw std::runtime_error("can't read data.csv");

	std::string line;
	bool throwError = false;

	std::getline(inputFile, line);
	if (line != "date | value")
		HandleError::handleError("invalid header format, should be : \"date | value\"", line, throwError);
}

void validateFormatInputFile(std::string line, bool throwError)
{
	if (line.size() < 13)
		HandleError::handleError("bad input", line, throwError);

	std::size_t found = line.find(" | ", 10);
	if (found == std::string::npos)
		HandleError::handleError("bad input", line, throwError);
}

void	BitcoinExchange::printWalletvalueByDate(std::string parseDate, double btcCount)
{
	std::map<std::string, double>::iterator it;

	it = _btcPriceByDate.lower_bound(parseDate);
	if (it->first != parseDate)
		--it;
	std::cout << parseDate << " => " << btcCount << " = " << (it->second * btcCount) << std::endl;
}

void BitcoinExchange::exchange(std::string inputFilename)
{
	std::fstream inputFile(inputFilename.c_str());
	if (!inputFile)
		throw std::runtime_error(std::string("can't open file : ") + inputFilename);

	std::string line;
	bool throwError = true;
	double btcCount;

	validateInputFileHeader(inputFile);

	while (std::getline(inputFile, line) && !inputFile.eof())
	{
		try {
			validateFormatInputFile(line, throwError);

			std::string parseDate = line.substr(0, 10);
			std::string parseValue = line.substr(13, line.size());

			Parser::validateDate(parseDate, throwError);
			btcCount = Parser::validateValue(parseValue, btcCount, throwError);
			if (btcCount > 1000)
				HandleError::handleError("too large number", parseValue, throwError);

			printWalletvalueByDate(parseDate, btcCount);

		} catch (std::exception &e) {
			std::cout << "exchange catch :" << e.what() << std::endl;
		}
	}
}
