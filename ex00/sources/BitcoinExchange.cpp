#include "BitcoinExchange.hpp"
#include <exception>
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <ctime>

BitcoinExchange::BitcoinExchange()
{
	throw std::runtime_error("BitcoinExchange requires an input file");
}

BitcoinExchange::BitcoinExchange(std::string inputFilename)
:
	_inputFilename(inputFilename),
	_btcPriceByDate()
{}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& copy)
:
	_inputFilename(copy._inputFilename),
	_btcPriceByDate(copy._btcPriceByDate)
{}

BitcoinExchange::~BitcoinExchange()
{}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this == &other)
		return *this;

	_inputFilename = other._inputFilename;
	_btcPriceByDate = other._btcPriceByDate;
	return *this;
}

void warningMsg(std::string msg)
{
	std::cout << msg << std::endl;
}

void	validateHeader(std::fstream& dataCsv)
{
	if (!dataCsv.is_open())
		throw std::runtime_error("::validateHeader : data.csv should be open");

	std::string readBuf;
	std::getline(dataCsv, readBuf);
	if (readBuf != "date,exchange_rate")
		warningMsg("<Warning> data.csv header should be : \"date,exchange_rate\"");
}

void handleError(std::string errMsg, std::string wrongInput, int throwError)
{
	std::string errorMessage = "Error: " + errMsg + " => " + wrongInput;

	if (throwError)
		throw std::runtime_error(errorMessage);
	std::cout << errorMessage << std::endl;
}

//validateDate.cpp beg

bool toInt(int &num, std::string str)
{
	std::stringstream ss(str);
	ss >> num;
	if (!ss.eof() || ss.fail())
		return 0;
	return 1;
}

bool validateYear(struct tm date)
{
	if (date.tm_year >= 2009 && date.tm_year <= 2026)
		return 1;
	return 0;
}

bool validateMonth(struct tm date)
{
	if (date.tm_mon >= 1 && date.tm_mon <= 12)
		return 1;
	return 0;
}

int leapYear(int year)
{
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    return 29;
	return 28;
}

bool validateDay(struct tm date)
{
	int month = date.tm_mon - 1;
	int day = date.tm_mday;
	int feb = leapYear(date.tm_year);
	int daysPerMonth[] = {31, feb, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (day <= daysPerMonth[month])
		return 1;
	return 0;
}

bool validateDate(std::string readDate, int throwError)
{
	struct tm date;

	std::cout << "readDate : " << readDate << std::endl;

	if (readDate.size() != 10 || readDate[4] != '-' || readDate[7] != '-') {
		handleError("bad format", readDate, throwError);
		return 0;
	}

	if ( !toInt(date.tm_year, readDate.substr(0, 4))
		|| !toInt(date.tm_mon, readDate.substr(5, 2))
		|| !toInt(date.tm_mday, readDate.substr(8, 2)))
	{
		handleError("non-numeric characters in date", readDate, throwError);
	}

	std::cout << "year :" << date.tm_year << std::endl; 
	std::cout << "month :" << date.tm_mon << std::endl; 
	std::cout << "day :" << date.tm_mday << std::endl; 

	if ( !validateYear(date)
		|| !validateMonth(date)
		|| !validateDay(date))
	{
		handleError("invalid year, month, day value", readDate, throwError);
	}

	return 1;
}

//validateDate.cpp end

bool toDouble(double &num, std::string str)
{
	std::stringstream ss(str);
	ss >> num;
	if (!ss.eof() || ss.fail())
		return 0;
	return 1;
}

bool validateValue(std::string readValue, double& value, int throwError)
{
	(void)value;
	(void)throwError;
	std::cout << "readValue :" << readValue << std::endl;

	if (!toDouble(value, readValue))
	{
		handleError("invalid value format", readValue, throwError);
		return 0;
	}
	return 1;
}

void	BitcoinExchange::insertKeyValueInDB(std::string key, double value)
{
	_btcPriceByDate[key] = value;
}

void	BitcoinExchange::validateLines(std::fstream& dataCsv)
{
	if (!dataCsv.is_open())
		throw std::runtime_error("::validateLines : data.csv should be open");

	std::string	readDate;
	std::string	readValue;
	double	value = -1;
	int	throwError = 0;

	while (std::getline(dataCsv, readDate, ',') && !dataCsv.eof())
	{
		std::getline(dataCsv, readValue);

		if ( validateDate(readDate, throwError)
			&& validateValue(readValue, value, throwError))
		{
			insertKeyValueInDB(readDate, value);
		}
		std::cout << std::endl;
	}
}

void	BitcoinExchange::createDB()
{
	std::fstream dataCsv("data.csv");
	if (!dataCsv)
		throw std::runtime_error("can't open file : data.csv");

	validateHeader(dataCsv);
	validateLines(dataCsv);

	dataCsv.close();
}

void	BitcoinExchange::printDB()
{
	std::map<std::string, double>::iterator it;

	for (it = _btcPriceByDate.begin(); it != _btcPriceByDate.end(); ++it)
		std::cout << it->first << " => " << it->second << std::endl;
}


