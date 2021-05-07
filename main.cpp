
#include "mainwindow.h"

#include <QApplication>
#include <iostream>
#include <stdlib.h> 
#include <locale.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    
    setlocale (LC_ALL,"en_US.utf8");


    w.show();
    return a.exec();
}

// int main() {


	
// 	std::cout << "Hello, Shunting Yard!" << std::endl;

// 	std::cout << "Input equation: ";

// 	std::string eqn;
// 	std::getline(std::cin, eqn);

// 	//ShuntingYard::variables["x"] = 15;

	
// 	ReversePolishNotation::RPN rpn = ReversePolishNotation::reversePolishNotation(eqn);
// 	//ShuntingYard::RPN rpn = ShuntingYard::reversePolishNotation(eqn.c_str());
	
// 	std::shared_ptr<ShuntingYard::Node> tree = ShuntingYard::parse(rpn);
// 	std::cout << "= " << ShuntingYard::calculate(tree) << std::endl;

// 	return 0;
// }