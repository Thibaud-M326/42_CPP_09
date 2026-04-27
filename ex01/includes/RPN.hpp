#ifndef RPN_HPP
#define RPN_HPP

#include <string>

class Rpn {
	private:
		Rpn(const Rpn& copy);
		Rpn& operator=(const Rpn& other);

		double	evaluateRpn(std::string expression);
		void		isValidExpression(std::string expression);
	public:
		Rpn();
		~Rpn();

		double evaluate(std::string expression);
};

#endif
