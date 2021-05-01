#include <string>
class Calculator
{
private:
    int result,input;
public:
    Calculator(/* args */):result{0},input{0}{}
    int calculate(const std::string& operation);
    ~Calculator()=default;
};


