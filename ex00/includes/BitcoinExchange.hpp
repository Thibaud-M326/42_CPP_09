#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <fstream>
#include <string>
#include <map>

typedef std::map<std::string, double> PriceByDateMap;

class BitcoinExchange {
	private:
		PriceByDateMap	_btcPriceByDate;

		void	insertInPriceByDateMap(std::string key, double value);

		void	validateDataHeader(std::fstream& dataCsv);
		void	validateFormatData(std::string line);

	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& copy);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();

		void createDB();
		void exchange(std::string inputFilename);
		void printDB();
		static void	handleError(std::string errMsg, std::string wrongInput, int throwError);
};

#endif
