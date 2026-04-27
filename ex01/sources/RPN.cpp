#include "RPN.hpp"
#include <stdexcept>
#include <stack>
#include <iostream>

Rpn::Rpn()
{}

Rpn::~Rpn()
{}

void	Rpn::isValidExpression(std::string expression)
{
	std::string::iterator it;
	size_t countNum = 0;
	size_t countOper = 0;

	for (it = expression.begin(); it != expression.end(); ++it)
	{
		if (std::isdigit(*it))
			countNum++;
		else if ((*it == '+' || *it == '-' || *it == '*' || *it == '/') && countNum > 1)
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

double Rpn::evaluateRpn(std::string expression)
{
	std::stack<double> rpn;

	std::string::iterator it;
	for (it = expression.begin(); it != expression.end(); ++it)
	{
		if (std::isdigit(*it))
			rpn.push(*it - '0');
		else if (*it == '+' || *it == '-' || *it == '*' || *it == '/')
		{
			double b = rpn.top();
			rpn.pop();
			double a = rpn.top();
			rpn.pop();

			if (*it == '+')
				rpn.push(a + b);
			if (*it == '*')
				rpn.push(a * b);
			if (*it == '-')
				rpn.push(a - b);
			if (*it == '/')
			{
				if (b != 0)
					rpn.push(a / b);
				else
					throw std::runtime_error(std::string("can divide by 0 :") + expression);
			}
		}
	}
	std::cout << rpn.top() << std::endl;
	return 0;
}

double Rpn::evaluate(std::string expression) 
{
	isValidExpression(expression);
	return evaluateRpn(expression);
}
