#include "ReversePolishNotation.h"
#include <stack>
#include <iostream>
namespace ReversePolishNotation
{
    bool isLeftAssociative(std::string str) {
		return g_BinaryFunctions[str].m_Left;
	}
    // get function precedence
	short getPrecedence(std::string str) {
		if (isContain<std::string>(keys(g_BinaryFunctions), str)) {
			return g_BinaryFunctions[str].m_Precendence;
		}

		return 0;
	}

    bool isNumber(char c,bool decimal, bool negative){
        if (c >= '0' && c <= '9') {
			return true;
		}
		else if (!decimal && c == '.') {
			return true;
		}
		else if (!negative && c == '-') {
			return true;
		}

		return false;
    }
    
    std::string findElement(const int& index, const std::string& equation, std::vector<std::string> list){
        for (std::string item : list) {
			int n = (int)item.size();
			if (equation.substr(index, n) == item) {
				return item;
			}
		}
		return "";
    }
    RPN reversePolishNotation(const std::string& equation){
        std::vector<std::string> queue;
        std::stack<std::string> stack;
        std::string element = "";
        std::pair<TokenTypes,TokenTypes> types = std::make_pair<TokenTypes,TokenTypes>(TokenTypes::UNKNOW,TokenTypes::UNKNOW);
        
        bool isDecimal = false;
        bool isNegative =false;
        const int equationLength = equation.size();
        
        for (int i = 0; i < equationLength; i++)
        {
            bool condition = true;
            char c = equation[i];
            if(isNumber(c)){
                types.first = TokenTypes::CONSTANT;
                if(c == '.'){
                    isDecimal = true;
                }else if(c == '-'){
                    isNegative = true;
                    if((types.second == TokenTypes::CONSTANT || types.second == TokenTypes::UNKNOW) && i != 0){
                        condition = false;
                    }
                }

                int startIndex = i;

                if(i<equationLength-1){
                    if(condition){
                        while(isNumber(equation[i+1],isDecimal,isNegative)){
                            if(equation[i+1]=='-'){
                                break;
                            }
                            i++;
                            if(i == equationLength-1){
                                break;
                            }
                            
                        }
                    }
                    
                }
                element = equation.substr(startIndex,i-startIndex+1);
                if(element=="-"){
                    types.first = TokenTypes::OPERATOR;
                }
                types.second = types.first;                

            }else{
                element = findElement(i,equation,g_FunctionNames);
                if(element != ""){
                    types.first = isContain<char>(g_Operators,element[0]) ? TokenTypes::OPERATOR : TokenTypes::FUNCTION;
                }
                else{
                    element = findElement(i,equation,g_ConstantNames);
                    if(element != ""){
                        types.first = TokenTypes::CONSTANT;
                    }else{
                        if(isContain<char>(g_LeftBrackets,equation[i])){
                            types.first = TokenTypes::LEFTPARANTESIS;
                            element = "(";
                        }else if(isContain<char>(g_RightBrackets,equation[i])){
                            types.first = TokenTypes::RIGHTPARANTESIS;
                            element = ")";                        
                        }else{
                            types.first = TokenTypes::UNKNOW;
                        }
                    }
                }
                i += element.size()-1;                
            }

            std::string last_stack = (stack.size() > 0) ? stack.top() : "";

            switch (types.first)
            {
            case TokenTypes::CONSTANT:
                queue.push_back(element);
                break;
            case TokenTypes::FUNCTION:
                stack.push(element);
                break;
            case TokenTypes::OPERATOR:
                if(stack.size()!=0){
                    while((
							(isContain<std::string>(g_FunctionNames, last_stack) && !isContain<char>(g_Operators, last_stack.c_str()[0])) ||
							getPrecedence(last_stack) > getPrecedence(element) || ((getPrecedence(last_stack) == getPrecedence(element)) &&	isLeftAssociative(last_stack))) &&
						    !isContain<char>(g_LeftBrackets, last_stack.c_str()[0])
                        ){
                        queue.push_back(stack.top());
                        stack.pop();
                        if(stack.size()==0){
                            break;
                        }
                        last_stack = stack.top();
                    }
                }
                stack.push(element);
                break;
            case TokenTypes::LEFTPARANTESIS:
                stack.push(element);
                break;
            case TokenTypes::RIGHTPARANTESIS:
                while(last_stack[0] != '('){
                    queue.push_back(stack.top());
                    stack.pop();
                    last_stack = stack.top();
                }
                stack.pop();
                break;
            default:
                return queue;
            }
            types.second = types.first;
        }
        while(stack.size()>0){
            queue.push_back(stack.top());
            stack.pop();
        }
        return queue;
    }

    std::map<std::string, Function> g_UnaryFunctions = {
        {"sin",Function(static_cast<double(*)(double)>(std::sin))}
    };

    std::map<std::string, Function> g_BinaryFunctions = {
		{ "+", Function([](double x, double y) -> double { return x + y; }, TokenTypes::OPERATOR, 2) },
		{ "-", Function([](double x, double y) -> double { return x - y; }, TokenTypes::OPERATOR, 2) },
		{ "*", Function([](double x, double y) -> double { return x * y; }, TokenTypes::OPERATOR, 3) },
		{ "/", Function([](double x, double y) -> double { return x / y; }, TokenTypes::OPERATOR, 3) },
		{ "^", Function(static_cast<double(*)(double,double)>(std::pow), TokenTypes::OPERATOR, 4, false) }
	};

    std::vector<std::string> g_FunctionNames = keys<Function>(g_UnaryFunctions, g_BinaryFunctions);

	// constants
	std::map<std::string, double> g_Constants = {
		{ "pi", std::atan(1) * 4 },
		{ "e", std::exp(1) }
	};

    std::vector<std::string> g_ConstantNames = keys<double>(g_Constants);
} // namespace ReversePolishNotation
//256+874159+878745sadasd+sad5589741-8895478-