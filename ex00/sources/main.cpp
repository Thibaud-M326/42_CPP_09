#include "BitcoinExchange.hpp"
#include <iostream>

int main (int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: ./btc <filename>" << std::endl;
		return 1;
	}

	try
	{
		BitcoinExchange btcEx;
		std::string inputFilename = argv[1];
		
		btcEx.createDB();
		// btcEx.printDB();
		btcEx.exchange(inputFilename);
	}
	catch (std::exception &e)
	{
		std::cout << "main catch :" <<  e.what() << std::endl;
	}
}

