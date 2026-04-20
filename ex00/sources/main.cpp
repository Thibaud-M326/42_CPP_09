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
		BitcoinExchange btcEx(argv[1]);
		
		btcEx.createDB();
		btcEx.printDB();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}

