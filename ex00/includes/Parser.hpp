#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>

class Parser {

	private:
		static bool	toInt(int &num, std::string str);
		static bool	validateYear(struct tm date);
		static bool	validateMonth(struct tm date);
		static int		leapYear(int year);
		static bool	validateDay(struct tm date);
		static bool	toDouble(double &num, std::string str);

	public:
		Parser();
		~Parser();

		static void validateDate(std::string readDate, int throwError);
		static double validateValue(std::string parseValue, double& value, int throwError);
};

#endif
