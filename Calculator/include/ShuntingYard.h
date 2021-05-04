#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <math.h>
#include <stack>
#include "ReversePolishNotation.h"

namespace ShuntingYard
{
    bool isContainsNumbers(const std::string& str);
    bool isNumber(const std::string& str);
    double getNumericalValue(const std::string& str);

    class Node{
    public:
        Node(const std::string& name, bool isFunction)
            :m_Name(name),m_isFunction(isFunction){}

        virtual double calculate(const double& x=0,const double& y=0)=0;

        std::string m_Name;
        bool m_isFunction;

        std::shared_ptr<Node> m_Left;
        std::shared_ptr<Node> m_Right;
    };

    class FunctionNode : public Node{
    public:
        FunctionNode(const std::string& name)
            :Node(name,true){}

        inline void setUnary(bool isUnary){
            this->m_isUnary=isUnary;
            this->m_Function= isUnary ? ReversePolishNotation::g_BinaryFunctions[m_Name]: ReversePolishNotation::g_BinaryFunctions[m_Name];
        }

        inline double calculate(const double& x, const double& y=0)override{return this->m_Function.calculate(x,y);}

        bool m_isUnary;
        ReversePolishNotation::Function m_Function;

    };

    class NumberNode : public Node{
    public:
        NumberNode(const std::string& name)
            :Node(name,false){}

        inline double calculate(const double& x=0, const double& y=0)override{return getNumericalValue(m_Name);};
    };
    
    std::shared_ptr<Node> parse(ReversePolishNotation::RPN rpn);

    double calculate(std::shared_ptr<Node> tree);

} // namespace ShuntingYard


#endif