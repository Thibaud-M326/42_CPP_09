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

		void	insertInPriceByDateMap(std::string key, double value);


		void	validateDataHeader(std::fstream& dataCsv);
		void	validateFormatData(std::string line);

	public:
		BitcoinExchange();
		BitcoinExchange(std::string inputFilename);
		BitcoinExchange(const BitcoinExchange& copy);
		~BitcoinExchange();

		BitcoinExchange& operator=(const BitcoinExchange& other);

		void createDB();
		void printDB();
		static void	handleError(std::string errMsg, std::string wrongInput, int throwError);
};

#endif
