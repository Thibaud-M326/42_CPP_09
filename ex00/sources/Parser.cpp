#include "Parser.hpp"
#include "BitcoinExchange.hpp"
#include "HandleError.hpp"
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

void Parser::validateDate(std::string readDate)
{
	struct tm date;

	if (readDate.size() != 10 || readDate[4] != '-' || readDate[7] != '-') 
		HandleError::handleError("bad format", readDate);

	if ( !toInt(date.tm_year, readDate.substr(0, 4))
		|| !toInt(date.tm_mon, readDate.substr(5, 2))
		|| !toInt(date.tm_mday, readDate.substr(8, 2)))
	{
		HandleError::handleError("non-numeric characters in date", readDate);
	}

	if ( !validateYear(date)
		|| !validateMonth(date)
		|| !validateDay(date))
	{
		HandleError::handleError("invalid year, month, day value", readDate);
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

double Parser::validateValue(std::string parseValue, double value)
{
	if (parseValue.empty())
		HandleError::handleError("invalid value format", "\"\"");

	if ( !toDouble(value, parseValue))
		HandleError::handleError("invalid value format", parseValue);

	if (value < 0)
		HandleError::handleError("not a positive number", parseValue);

	return value;
}
