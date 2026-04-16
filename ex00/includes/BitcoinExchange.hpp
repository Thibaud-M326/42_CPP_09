#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <fstream>
#include <string>
#include <map>

class BitcoinExchange {
	private:
		std::string	_inputFilename;
		std::map<std::string, double> _btcPriceByDate;

		void	validateLines(std::fstream& dataCsv);
		void	insertKeyValueInDB(std::string key, double value);

	public:
		BitcoinExchange();
		BitcoinExchange(std::string inputFilename);
		BitcoinExchange(const BitcoinExchange& copy);
		~BitcoinExchange();

		BitcoinExchange& operator=(const BitcoinExchange& other);

		void createDB();
		void printDB();
};

#endif
