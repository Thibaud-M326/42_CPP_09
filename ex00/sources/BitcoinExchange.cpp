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

