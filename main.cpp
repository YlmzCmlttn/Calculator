#include <iostream>
#include "ShuntingYard.h"
int main() {


	
	std::cout << "Hello, Shunting Yard!" << std::endl;

	std::cout << "Input equation: ";

	std::string eqn;
	std::getline(std::cin, eqn);

	//ShuntingYard::variables["x"] = 15;

	
	ReversePolishNotation::RPN rpn = ReversePolishNotation::reversePolishNotation(eqn);
	//ShuntingYard::RPN rpn = ShuntingYard::reversePolishNotation(eqn.c_str());
	
	std::shared_ptr<ShuntingYard::Node> tree = ShuntingYard::parse(rpn);
	std::cout << "= " << ShuntingYard::calculate(tree) << std::endl;

	return 0;
}