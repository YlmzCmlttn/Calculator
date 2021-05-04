#ifndef REVERSE_POLISH_NOTATION_H
#define REVERSE_POLISH_NOTATION_H
#include <vector>
#include <string>
#include <functional>
#include <map>
#include <memory>
#include <math.h>
namespace ReversePolishNotation
{
    typedef std::vector<std::string> RPN;

    

    RPN reversePolishNotation(const std::string& equation);

    enum class TokenTypes{
        CONSTANT,
        OPERATOR,
        FUNCTION,
        LEFTPARANTESIS,
        RIGHTPARANTESIS,
        UNKNOW
    };
    short getPrecedence(std::string str);
    bool isLeftAssociative(std::string str);
    bool isNumber(char c,bool decimal=false, bool negative=false);
    std::string findElement(const int& index, const char* equation, std::vector<std::string> list);

    template<typename T> bool isContain(const std::vector<T>& v,const T& x) {return std::find(v.begin(), v.end(), x) != v.end();}

    template <typename T> std::vector<std::string> keys(const std::map<std::string,T>& m){
        std::vector<std::string> ret;

		// push each key from each pair
		for (auto const& element : m) {
			ret.push_back(element.first);
		}

		return ret;
    }
    template <typename T> std::vector<std::string> keys(const std::map<std::string,T>& m1,const std::map<std::string,T>& m2){

		// get keys from each map
		std::vector<std::string> keySet1 = keys<T>(m1);
		std::vector<std::string> keySet2 = keys<T>(m2);

		// insert the second list into first
		keySet1.insert(keySet1.end(), keySet2.begin(), keySet2.end());

		// return result
		return keySet1;
    }

   
    typedef std::function<double(double)> UnaryFunction;
    typedef std::function<double(double,double)> BinaryFunction;

    class Function{        
    public:
        Function()
            :m_TokenType(TokenTypes::OPERATOR), m_Precendence(0), m_Left(true), m_Unary(true), m_UnaryFunction(nullptr), m_BinaryFunction(nullptr) {}
        Function(const UnaryFunction& func,const TokenTypes& type=TokenTypes::FUNCTION,const short& prec=0,const bool& left =true)
            :Function(type,prec,left,true){
                m_UnaryFunction = func;
            }
        Function(const BinaryFunction& func,const TokenTypes& type=TokenTypes::FUNCTION,const short& prec=0,const bool& left =true)
            :Function(type,prec,left,false){
                m_BinaryFunction = func;
            }

        inline double calculate(const double& x, const double& y = 0){ return this->m_Unary ? m_UnaryFunction(x) : m_BinaryFunction(x,y);}

        UnaryFunction m_UnaryFunction;
        BinaryFunction m_BinaryFunction;

        TokenTypes m_TokenType;

        short m_Precendence;

        bool m_Left;
        bool m_Unary;

    private:
        Function(const TokenTypes& type, const short& prec, const bool& left, const bool& unary)
            :m_TokenType{type}, m_Precendence{prec}, m_Left{left}, m_UnaryFunction{nullptr}, m_BinaryFunction{m_BinaryFunction}{}
    };

    extern std::map<std::string, Function> g_UnaryFunctions;

    extern std::map<std::string, Function> g_BinaryFunctions;

    extern std::vector<std::string> g_FunctionNames;

	// constants
	extern std::map<std::string, double> g_Constants;

    extern std::vector<std::string> g_ConstantNames;

	// operators
	const std::vector<char> g_Operators = { '+', '-', '/', '*', '^' };
	// left brackets
	const std::vector<char> g_LeftBrackets = { '(', '{', '[' };
	// right brackets
	const std::vector<char> g_RightBrackets = { ')', '}', ']' };
   

} // namespace ReversePolishNotation

#endif