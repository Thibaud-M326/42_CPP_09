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

void handleError(std::string errMsg, std::string wrongInput, int throwError)
{
	std::string errorMessage = "Error: " + errMsg + " => " + wrongInput;

	if (throwError)
		throw std::runtime_error(errorMessage);
	std::cout << errorMessage << std::endl;
}

bool toInt(int &num, std::string str)
{
	std::stringstream ss(str);
	ss >> num;
	if (!ss.eof() || ss.fail())
		return 0;
	return 1;
}

//un date valide c'est beaucoup de choses,
//deja c est des entier, format YYYY MM DD
//on vas donc essayer avec un string stream pk pas de prendre jusqu a '-' puis apres je vais faire les tests pour l anne,
//je suis oblige de prendre l annee le moi et la date pour verifier si ils sont juste, je prend dans une string jusqu a '-'
//je prend les date, je verifie les dates 
//2009-05-03
int validateDateKey(std::string keyDate, int throwError)
{
	int year = 0;
	int month = 0;
	int day = 0;

	std::cout << "KeyDate : " << keyDate << std::endl;

	if (keyDate.size() != 10 || keyDate[4] != '-' || keyDate[7] != '-') {
		handleError("bad input", keyDate, throwError);
		return 0;
	}

	if ( !toInt(year, keyDate.substr(0, 4))
		|| !toInt(month, keyDate.substr(5, 2))
		|| !toInt(day, keyDate.substr(8, 2)))
	{
		handleError("bad input", keyDate, throwError);
	}

	return 1;
}

void	validateLines(std::fstream& dataCsv)
{
	if (!dataCsv.is_open())
		throw std::runtime_error("::validateLines : data.csv should be open");

	std::string readDate;
	std::string readValue;
	int throwError = 0;

	while (std::getline(dataCsv, readDate, ',') && !dataCsv.eof())
	{
		std::getline(dataCsv, readValue);

		validateDateKey(readDate, throwError);
		// validateValue(readValue);

		// if (isValidKeyValue)
		// 	insertKeyValueInDB();
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
