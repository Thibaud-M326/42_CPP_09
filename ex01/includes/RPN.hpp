#ifndef RPN_HPP
#define RPN_HPP

#include <string>

class Rpn {
	private:
		Rpn(const Rpn& copy);
		Rpn& operator=(const Rpn& other);

	public:
		Rpn();
		~Rpn();

		double evaluate(std::string expression);
};

#endif
