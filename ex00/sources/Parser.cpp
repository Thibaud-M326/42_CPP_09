#include "Parser.hpp"
#include "BitcoinExchange.hpp"
#include <string>
#include <iostream>
#include <sstream>

bool Parser::toInt(int &num, std::string str)
{
	std::stringstream ss(str);
	ss >> num;
	if (!ss.eof() || ss.fail())
		return 0;
	return 1;
}

bool Parser::validateYear(struct tm date)
{
	if (date.tm_year >= 2009 && date.tm_year <= 2026)
		return 1;
	return 0;
}

bool Parser::validateMonth(struct tm date)
{
	if (date.tm_mon >= 1 && date.tm_mon <= 12)
		return 1;
	return 0;
}

int Parser::leapYear(int year)
{
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    return 29;
	return 28;
}

bool Parser::validateDay(struct tm date)
{
	int month = date.tm_mon - 1;
	int day = date.tm_mday;
	int feb = leapYear(date.tm_year);
	int daysPerMonth[] = {31, feb, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (day <= daysPerMonth[month])
		return 1;
	return 0;
}

void Parser::validateDate(std::string readDate, int throwError)
{
	struct tm date;

	std::cout << "readDate : " << readDate << std::endl;

	if (readDate.size() != 10 || readDate[4] != '-' || readDate[7] != '-') 
		BitcoinExchange::handleError("bad format", readDate, throwError);

	if ( !toInt(date.tm_year, readDate.substr(0, 4))
		|| !toInt(date.tm_mon, readDate.substr(5, 2))
		|| !toInt(date.tm_mday, readDate.substr(8, 2)))
	{
		BitcoinExchange::handleError("non-numeric characters in date", readDate, throwError);
	}

	std::cout << "year :" << date.tm_year << std::endl; 
	std::cout << "month :" << date.tm_mon << std::endl; 
	std::cout << "day :" << date.tm_mday << std::endl; 

	if ( !validateYear(date)
		|| !validateMonth(date)
		|| !validateDay(date))
	{
		BitcoinExchange::handleError("invalid year, month, day value", readDate, throwError);
	}
}

bool Parser::toDouble(double &num, std::string str)
{
	std::stringstream ss(str);
	ss >> num;
	if (!ss.eof() || ss.fail())
		return 0;
	return 1;
}

double Parser::validateValue(std::string parseValue, double& value, int throwError)
{
	if (parseValue.empty())
		BitcoinExchange::handleError("invalid value format", "\"\"", throwError);
	if ( !toDouble(value, parseValue))
		BitcoinExchange::handleError("invalid value format", parseValue, throwError);
	return value;
}
