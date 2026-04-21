#include "RPN.hpp"
#include <stdexcept>

Rpn::Rpn()
{}

Rpn::~Rpn()
{}

void	isValidExpression(std::string expression)
{
	std::string::iterator it;
	size_t countNum = 0;
	size_t countOper = 0;

	for (it = expression.begin(); it != expression.end(); ++it)
	{
		if (std::isdigit(*it))
			countNum++;
		else if (*it == '+' || *it == '-' || *it == '*' || *it == '/')
			countOper++;
		else if (*it == ' ')
			continue;
		else
			throw std::runtime_error(std::string("bad expression input :") + expression);
	}

	if ((countNum - countOper) != 1)
		throw std::runtime_error(std::string("bad expression input, bad num opr count :") + expression);

	for (it = expression.begin(); it != expression.end(); ++it)
	{
		if (it != (expression.end() - 1))
			if ((std::isdigit(*it) || *it == '+' || *it == '-' || *it == '*' || *it == '/') && *(it + 1) != ' ')
				throw std::runtime_error(std::string("bad expression input, no space after num :") + expression);
	}
}

double Rpn::evaluate(std::string expression) 
{
	isValidExpression(expression);
	return 0;
}
