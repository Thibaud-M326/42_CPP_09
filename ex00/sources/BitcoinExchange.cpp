#include "BitcoinExchange.hpp"
#include <exception>
#include <fstream>

BitcoinExchange::BitcoinExchange()
{
	throw std::runtime_error("BitcoinExchange requires an input file");
}

BitcoinExchange::BitcoinExchange(std::string inputFilename)
:
	_inputFilename(inputFilename)
{
	openFile(_inputFile, _inputFilename);
	openFile(_dataFile, "data.csv");
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& copy)
:
	_inputFilename(copy._inputFilename)
{
	openFile(_inputFile, _inputFilename);
	openFile(_dataFile, "data.csv");
}

BitcoinExchange::~BitcoinExchange()
{
  if (_inputFile.is_open())
		_inputFile.close();

  if (_dataFile.is_open())
		_dataFile.close();
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this == &other)
		return *this;

	openFile(_inputFile, _inputFilename);
	openFile(_dataFile, "data.csv");
}

void	BitcoinExchange::openFile(std::fstream& fs, std::string filename)
{
	fs.open(filename.c_str());

	if (!fs.is_open())
		throw std::runtime_error(std::string("can't open file : ") + filename);
}
