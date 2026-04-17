#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <fstream>
#include <string>
#include <map>

typedef std::map<std::string, double> PriceByDateMap;

class BitcoinExchange {
	private:
		std::string			_inputFilename;
		PriceByDateMap	_btcPriceByDate;

		bool	validateLine(std::string& readDate, std::string& readValue, PriceByDateMap& parsedKeyValue, bool& throwError);
		void	insertInPriceByDateMap(std::string key, double value);

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
