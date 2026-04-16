#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <fstream>
#include <string>
#include <map>

class BitcoinExchange {
	private:
		std::string	_inputFilename;
		std::map<std::string, double> btcPriceByDate;

	public:
		BitcoinExchange();
		BitcoinExchange(std::string inputFilename);
		BitcoinExchange(const BitcoinExchange& copy);
		~BitcoinExchange();

		BitcoinExchange& operator=(const BitcoinExchange& other);

		void createDB();
};

#endif
