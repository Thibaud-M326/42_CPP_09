#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <fstream>
#include <string>

class BitcoinExchange {
	private:
		std::string		_inputFilename;
		std::fstream	_inputFile;
		std::fstream	_dataFile;

		void openFile(std::fstream&, std::string);

	public:
		BitcoinExchange();
		BitcoinExchange(std::string inputFilename);
		BitcoinExchange(const BitcoinExchange& copy);
		~BitcoinExchange();

		BitcoinExchange& operator=(const BitcoinExchange& other);
};

#endif
