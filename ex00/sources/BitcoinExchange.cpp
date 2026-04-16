#include "BitcoinExchange.hpp"
#include <exception>
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>

BitcoinExchange::BitcoinExchange()
{
	throw std::runtime_error("BitcoinExchange requires an input file");
}

BitcoinExchange::BitcoinExchange(std::string inputFilename)
:
	_inputFilename(inputFilename)
{}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& copy)
:
	_inputFilename(copy._inputFilename)
{}

BitcoinExchange::~BitcoinExchange()
{}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this == &other)
		return *this;

	_inputFilename = other._inputFilename;
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

void printError(std::string errMsg, int print)
{
	if (!print)
		return;
	std::cout << "Error: " << errMsg << std::endl;
}

//un date valide c'est beaucoup de choses,
//deja c est des entier, format YYYY MM DD
//on vas donc essayer avec un string stream pk pas de prendre jusqu a '-' puis apres je vais faire les tests pour l anne,
//je suis oblige de prendre l annee le moi et la date pour verifier si ils sont juste, je prend dans une string jusqu a '-'
//je prend les date, je verifie les dates 
//2009-05-03
int validateDateKey(std::string keyDate, int throwErr)
{
	if (keyDate.size() > 10 || keyDate[4] != '-' || keyDate[7] != '-')

	(void)throwErr;

	std::cout << "KeyDate : " << keyDate << std::endl;
	// std::string year = extractDate();
	// std::string month = extractDate();
	// std::string day = extractDate();
	//
	// stringstream ss(KeyDate);
	
	return 0;
}

void	validateLines(std::fstream& dataCsv)
{
	if (!dataCsv.is_open())
		throw std::runtime_error("::validateLines : data.csv should be open");

	std::string readDate;
	std::string readValue;
	int					isValidKeyValue;
	int					throwErr = 0;

	(void)isValidKeyValue;
	while (std::getline(dataCsv, readDate, ',') && !dataCsv.eof())
	{
		std::getline(dataCsv, readValue);

		validateDateKey(readDate);
		// validateValue(readValue);
		//
		// insertKeyValueInDB();
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
