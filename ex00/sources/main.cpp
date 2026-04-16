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
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}

//1 - createDatabase
//on tente d ouvrir la db data.csv, on throw si on y arrive pas
//ensuite on vas parser chaque ligne, la premiere va etre date,exchange_rate
//on veut savoir si une date est correcte avant de l inserer : 
//ex : YYYY-MM-DD : 2001-05-25 OK ---- 2001-52-25 --- non, annee bisextile pour fevrier a check avant d'inserer
//ensuite on va parser chaque ligne, on va stocker la partie date, et la partie double avant puis on insere que si la date et le float sont correcte
//on vas verifier si chaque ligne est correcte, pour le data.csv on va simplement rien faire, juste un vas prendre les data
//et essayer des les inserer dans la db, si le format de la ligne est pas bon on le met pas dans la db mais aucun affichage de warning ?
//on vas mettre nos data dans une map<string, float>, la map est d office dans le bon ordre
//
//2 - 
//ensuite j ai un deuxieme fichier, le fichier input, je dois passer sur chaques ligne :
//la premiere c'est == date | value
//ensuite on vas passer sur chaque ligne, on ne veut pas les mettre dans une db on veut juste verifier si une ligne est correcte et multiplier son montant
//a la date correspondante dans la db et si la date ne correspond pas on met la date d avant (lower bound)
//si on a pas uL
