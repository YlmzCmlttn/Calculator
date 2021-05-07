#include "ShuntingYard.h"
#include <stack>
#include <cstring>
#include <iostream>
namespace ShuntingYard
{
	bool isContainsNumbers(const std::string& str) {
		// try to prove wrong
		bool isDecimal = false;
		bool isNegative = false;
		for (char c : std::string(str)) {
			if (!ReversePolishNotation::isNumber(c, isDecimal, isNegative)) {
				return false;
			}
			if (c == '.') {
				isDecimal = true;
			} if (c == '-') {
				isNegative = true;
			}
		}

		return true;
	}
    bool isNumber(const std::string& str) {
		return ReversePolishNotation::isContain<std::string>(ReversePolishNotation::g_ConstantNames, str) ||
			std::strcmp(str.c_str(), ".") == 0 ||
			isContainsNumbers(str);
	}
	double getNumericalValue(const std::string& str) {
		if (ReversePolishNotation::isContain<std::string>(ReversePolishNotation::g_ConstantNames, str)) {
			return ReversePolishNotation::g_Constants[str];
		}
		else {
			return std::atof(str.c_str());
		}
	}

	std::shared_ptr<Node> parse(ReversePolishNotation::RPN rpn){
        std::stack<std::shared_ptr<Node>> stack;
        for (std::string item : rpn){
            if(isNumber(item)&& item !="-"){
                stack.push(std::make_shared<NumberNode>(item));
            }else{
                std::shared_ptr<FunctionNode> f = std::make_shared<FunctionNode>(item);
                if(ReversePolishNotation::isContain<std::string>(ReversePolishNotation::keys(ReversePolishNotation::g_BinaryFunctions),item)){
                    f->setUnary(false);
                    f->m_Right = stack.top();
                    stack.pop();
                    f->m_Left = stack.top();
                    stack.pop();
                }
                else if(ReversePolishNotation::isContain<std::string>(ReversePolishNotation::keys(ReversePolishNotation::g_UnaryFunctions),item)){
                    f->setUnary(true);
                    f->m_Left = stack.top();
                    stack.pop();
                }
                stack.push(f);
            }
        }
        if(stack.size() == 0){
            return nullptr;
        }

        return stack.top();
    }

    double calculate(std::shared_ptr<Node> tree){
        if(tree->m_isFunction){
            std::shared_ptr<FunctionNode> ftree = std::static_pointer_cast<FunctionNode>(tree);
            if(ftree->m_isUnary){
                return ftree->calculate(calculate(tree->m_Left));
            }else{
                return ftree->calculate(calculate(tree->m_Left),calculate(tree->m_Right));
            }
        }else{
            std::shared_ptr<NumberNode> numtree = std::static_pointer_cast<NumberNode>(tree);
            return numtree->calculate();
        }
	}
	
} // namespace ShuntingYard
