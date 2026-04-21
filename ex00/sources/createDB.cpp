#include "BitcoinExchange.hpp"
#include "HandleError.hpp"
#include "Parser.hpp"

void	BitcoinExchange::validateDataHeader(std::fstream& dataCsv)
{
	if (!dataCsv.is_open())
		throw std::runtime_error("can't read data.csv");

	std::string line;
	double throwError = true;

	std::getline(dataCsv, line);
	if (line != "date,exchange_rate")
		HandleError::handleError("invalid header format, should be : \"date,exchange_rate\"", line, throwError);
}

void	BitcoinExchange::validateFormatData(std::string line)
{
	int throwError = 1;

	if (line[10] != ',')
		HandleError::handleError("invalid data format", line, throwError);
}

void	BitcoinExchange::createDB()
{
	std::fstream dataCsv("data.csv");
	if (!dataCsv)
		throw std::runtime_error("can't open file : data.csv");

	std::string line;
	bool throwError = true;
	double value = 0;

	validateDataHeader(dataCsv);
	while (std::getline(dataCsv, line) && !dataCsv.eof())
	{
		validateFormatData(line);
	
		std::string parseDate = line.substr(0, 10);
		std::string parseValue = line.substr(11, line.size());

		Parser::validateDate(parseDate, throwError);
		value = Parser::validateValue(parseValue, value, throwError);

		if (value < 0)
			HandleError::handleError("not a positive number", line, throwError);

		insertInPriceByDateMap(parseDate, value);
	}
	dataCsv.close();
}
