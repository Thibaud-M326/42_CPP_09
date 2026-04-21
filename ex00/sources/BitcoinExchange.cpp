#include "BitcoinExchange.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <ctime>

BitcoinExchange::BitcoinExchange()
:
	_btcPriceByDate()
{}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& copy)
:
	_btcPriceByDate(copy._btcPriceByDate)
{}

BitcoinExchange::~BitcoinExchange()
{}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this == &other)
		return *this;

	_btcPriceByDate = other._btcPriceByDate;
	return *this;
}

void	BitcoinExchange::insertInPriceByDateMap(std::string date, double value)
{
	_btcPriceByDate[date] = value;
}

void	BitcoinExchange::printDB()
{
	std::map<std::string, double>::iterator it;

	for (it = _btcPriceByDate.begin(); it != _btcPriceByDate.end(); ++it)
		std::cout << it->first << " => " << it->second << std::endl;
}

