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

	std::getline(inputFile, line);
	if (line != "date | value")
		HandleError::handleError("invalid header format, should be : \"date | value\"", line);
}

void validateFormatInputFile(std::string line)
{
	if (line.size() < 13)
		HandleError::handleError("bad input", line);

	std::size_t found = line.find(" | ", 10);
	if (found == std::string::npos)
		HandleError::handleError("bad input", line);
}

void	BitcoinExchange::printWalletvalueByDate(std::string parseDate, double btcCount)
{
	std::map<std::string, double>::iterator it;

	it = _btcPriceByDate.lower_bound(parseDate);
	if (it->first != parseDate)
	{
		if (it == _btcPriceByDate.begin())
			HandleError::handleError("no date matches", parseDate);
		--it;
	}
	std::cout << parseDate << " => " << btcCount << " = " << (it->second * btcCount) << std::endl;
}

void BitcoinExchange::exchange(std::string inputFilename)
{
	std::fstream inputFile(inputFilename.c_str());
	if (!inputFile)
		throw std::runtime_error(std::string("can't open file : ") + inputFilename);

	std::string line;
	double btcCount;

	validateInputFileHeader(inputFile);

	while (std::getline(inputFile, line) && !inputFile.eof())
	{
		try {
			validateFormatInputFile(line);

			std::string parseDate = line.substr(0, 10);
			std::string parseValue = line.substr(13, line.size());

			Parser::validateDate(parseDate);
			btcCount = Parser::validateValue(parseValue, btcCount);
			if (btcCount > 1000)
				HandleError::handleError("too large number", parseValue);

			printWalletvalueByDate(parseDate, btcCount);
		} 
		catch (std::exception &e) {
			std::cout << e.what() << std::endl;
		}
	}
}
