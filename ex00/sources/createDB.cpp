#include "BitcoinExchange.hpp"
#include "HandleError.hpp"
#include "Parser.hpp"

void	BitcoinExchange::validateDataHeader(std::fstream& dataCsv)
{
	if (!dataCsv.is_open())
		throw std::runtime_error("can't read data.csv");

	std::string line;

	std::getline(dataCsv, line);
	if (line != "date,exchange_rate")
		HandleError::handleError("invalid header format, should be : \"date,exchange_rate\"", line);
}

void	BitcoinExchange::validateFormatData(std::string line)
{
	if (line.size() < 11)
		HandleError::handleError("invalid data format", line);

	if (line[10] != ',')
		HandleError::handleError("invalid data format", line);
}

void	BitcoinExchange::createDB()
{
	std::fstream dataCsv("data.csv");
	if (!dataCsv)
		throw std::runtime_error("can't open file : data.csv");

	std::string line;
	double value = 0;

	validateDataHeader(dataCsv);
	while (std::getline(dataCsv, line) && !dataCsv.eof())
	{
		validateFormatData(line);
	
		std::string parseDate = line.substr(0, 10);
		std::string parseValue = line.substr(11, line.size());

		Parser::validateDate(parseDate);
		value = Parser::validateValue(parseValue, value);

		if (value < 0)
			HandleError::handleError("not a positive number", line);

		insertInPriceByDateMap(parseDate, value);
	}
	dataCsv.close();
}
