#ifndef SHUNTING_YARD
#define SHUNTING_YARD
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <math.h>

namespace ShuntingYard
{
    typedef std::vector<std::string> RPN;

    typedef std::function<double(const double&)> UnaryFunction;
    typedef std::function<double(const double&,const double&)> BinaryFunction;

    enum class TokenTypes{
        CONSTANT,
        OPERATOR,
        FUNCTION,
        LEFTPARANTESIS,
        RIGHTPARANTESIS,
        UNKNOW
    };

    template <typename T> bool isContains(const std::vector<T>&,const T&);

    template <typename T> std::vector<std::string> keys(const std::map<std::string,T>&);
    template <typename T> std::vector<std::string> keys(const std::map<std::string,T>&,const std::map<std::string,T>&);

    bool isNumber(const char& c, const bool& acceptDecimal=true,const bool& acceptNegative = true);
    bool isNumber(const char* str);
    bool containsNumbers(const char* str);

    double getNumericalVal(const char* str);

    bool isFunction(const std::string& str);

    bool isLeftAssociative(const std::string& str);

    short getPrecendence(const std::string& str);

    std::string findElement(const int&,const char*, const std::vector<std::string>&);

    class Function{        
    public:
        Function():m_TokenType(TokenTypes::OPERATOR), m_Precendence(0), m_Left(true), unary(true), u_eval(nullptr), b_eval(nullptr) {}
        Function(const UnaryFunction& func,const TokenTypes& type=TokenTypes::FUNCTION,const short& prec=0,const bool& left =true);
        Function(const BinaryFunction& func,const TokenTypes& type=TokenTypes::FUNCTION,const short& prec=0,const bool& left =true);

        double calculate(const double& x, const double& y = 0);

        UnaryFunction m_UnaryFunction;
        BinaryFunction m_BinaryFunction;

        TokenTypes m_TokenType;

        short m_Precendence;

        bool m_Left;
        bool m_Unary;

    private:
        Function(const TokenTypes& type, const short& prec, const bool& left, const bool& unary);
    };

    const std::map<std::string, Function> unary_functions = {
        {"sin",Function(std::sin)}
    };

    const std::map<std::string, Function> binary_functions = {
		{ "+", Function([](double x, double y) -> double { return x + y; }, TokenTypes::OPERATOR, 2) },
		{ "-", Function([](double x, double y) -> double { return x - y; }, TokenTypes::OPERATOR, 2) },
		{ "*", Function([](double x, double y) -> double { return x * y; }, TokenTypes::OPERATOR, 3) },
		{ "/", Function([](double x, double y) -> double { return x / y; }, TokenTypes::OPERATOR, 3) },
		{ "^", Function(std::pow, TokenTypes::OPERATOR, 4, false) }
	};

    const std::vector<std::string> functionNames = keys<Function>(unary_functions, binary_functions);

	// constants
	const std::map<std::string, double> constants = {
		{ "pi", std::atan(1) * 4 },
		{ "e", std::exp(1) }
	};


    const std::vector<std::string> constantNames = keys<double>(constants);

	// variables
	const std::map<std::string, double> variables;

	// operators
	const std::vector<char> operators = { '+', '-', '/', '*', '^' };
	// left brackets
	const std::vector<char> leftBrackets = { '(', '{', '[' };
	// right brackets
	const std::vector<char> rightBrackets = { ')', '}', ']' };



    class Node {
	public:
		Node(std::string name, bool isFunc)
			: name(name), isFunc(isFunc) {}

		double eval(double x = 0, double y = 0);

		std::string name;
		bool isFunc;

		Node* right;
		Node* left;
	};

	// function node class
	class FuncNode : public Node {
	public:
		FuncNode(std::string name)
			: Node(name, true) {}

		// set type of function and then assign callback
		void setUnary(bool isUnary) {
			this->isUnary = isUnary;

			this->func = isUnary ? unary_functions[name] : binary_functions[name];
		}

		// evaluate
		double eval(double x, double y = 0) {
			return this->func.calculate(x, y);
		}

		bool isUnary;
		Function func;
	};

	// number node class
	class NumNode : public Node {
	public:
		NumNode(std::string name)
			: Node(name, false) {}

		// return numerical value
		double eval(double x = 0, double y = 0) {
			return getNumericalVal(name.c_str());
		}
	};



} // namespace ShuntingYard


#endif