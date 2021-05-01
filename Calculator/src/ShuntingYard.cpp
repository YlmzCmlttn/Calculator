#include "ShuntingYard.h"
#include <stack>
#include <cstring>


namespace ShuntingYard
{
    RPN reversePolishNotation(const char* eqn) {
		std::vector<std::string> queue;
		std::stack<std::string> stack;

		std::string obj = "";
		TokenTypes type = TokenTypes::UNKNOW;
		TokenTypes prevType = TokenTypes::UNKNOW; // negative sign detection

		bool acceptDecimal = true;
		bool acceptNegative = true;

		// token reading and detection
		for (int i = 0, eqLen = (int)std::strlen(eqn); i < eqLen; i++) {
			char t = eqn[i];

			// skip spaces and commas
			if (t == ' ' || t == ',') {
				//prevType = TokenTypes::ELSE;
				continue;
			}

			// classify token
			if (isNumber(t)) {
				type = TokenTypes::CONSTANT;
				if (t == '.') {
					acceptDecimal = false;
				}
				else if (t == '-') {
					acceptNegative = false;
				}

				int startI = i;
				if (i < eqLen - 1) {
					while (isNumber(eqn[i + 1], acceptDecimal, acceptNegative)) {
						i++;
						if (i >= eqLen - 1) {
							break;
						}
					}
				}
				obj = std::string(eqn).substr(startI, i - startI + 1);

				// subtraction sign detection
				if (obj == "-") {
					type = TokenTypes::OPERATOR;
				}
			}
			else {
				obj = findElement(i, eqn, functionNames);
				if (obj != "") {
					// found valid object
					type = isContains<char>(operators, obj[0]) ? TokenTypes::OPERATOR : TokenTypes::FUNCTION;
				}
				else {
					obj = findElement(i, eqn, constantNames);
					if (obj != "") {
						// found valid object
						type = TokenTypes::CONSTANT;
					}
					else {
						obj = findElement(i, eqn, keys<double>(variables));
						if (obj != "") {
							type = TokenTypes::CONSTANT;
						}
						else if (isContains<char>(leftBrackets, t)) {
							type = TokenTypes::LEFTPARANTESIS;
							obj = "(";
						}
						else if (isContains<char>(rightBrackets, t)) {
							type = TokenTypes::RIGHTPARANTESIS;
							obj = ")";
						}
						else {
							type = TokenTypes::UNKNOW;
						}
					}
				}
				i += obj.size() - 1;
			}

			// do something with the token
			const char* last_stack = (stack.size() > 0) ? stack.top().c_str() : "";
			switch (type) {
			case TokenTypes::CONSTANT:
				queue.push_back(obj);
				break;
			case TokenTypes::FUNCTION:
				stack.push(obj);
				break;
			case TokenTypes::OPERATOR:
				if (stack.size() != 0) {
					while (
						(
							(contains<std::string>(functionNames, last_stack) &&
								!contains<char>(operators, last_stack[0])) ||
							getPrecedence(last_stack) > getPrecedence(obj) ||
							((getPrecedence(last_stack) == getPrecedence(obj)) &&
								isLeftAssociative(last_stack))
							) &&
						!contains<char>(leftBrackets, last_stack[0])
						) {
						queue.push_back(stack.top());
						stack.pop();
						if (stack.size() == 0) {
							break;
						}
						last_stack = stack.top().c_str();
					}
				}
				stack.push(obj);
				break;
			case TokenTypes::LPAREN:
				stack.push(obj);
				break;
			case TokenTypes::RPAREN:
				while (last_stack[0] != '(') {
					queue.push_back(stack.top());
					stack.pop();
					last_stack = stack.top().c_str();
				}
				stack.pop();
				break;
			default:
				return queue;
			}

			// reset type
			prevType = type;
		}

		while (stack.size() > 0) {
			queue.push_back(stack.top());
			stack.pop();
		}

		return queue;
	}

    Node* parse(RPN rpn) {
		std::stack<Node*> stack;

		for (std::string item : rpn) {
			if (isNumber(item.c_str()) && item != "-") {
				// push number node
				stack.push(new NumNode(item));
			}
			else {
				// function
				FuncNode* f = new FuncNode(item);
				if (contains<std::string>(keys(binary_functions), item)) {
					f->setUnary(false);
					// set children of node
					f->right = stack.top();
					stack.pop();
					f->left = stack.top();
					stack.pop();
				}
				else if (contains<std::string>(keys(unary_functions), item)) {
					f->setUnary(true);
					// set child of node
					f->left = stack.top();
					stack.pop();
				}
				stack.push(f);
			}
		}

		if (stack.size() == 0) {
			return nullptr;
		}

		return stack.top();
	}

	// evaluate tree
	double eval(Node* tree) {
		if (tree->isFunc) {
			FuncNode* ftree = (FuncNode*)tree;
			if (ftree->isUnary) {
				return ftree->eval(eval(tree->left));
			}
			else {
				return ftree->eval(eval(tree->left), eval(tree->right));
			}
		}
		else {
			// number node
			return ((NumNode*)tree)->eval();
		}
	}

	/*
		Utility function definitions
	*/

	// determine if vector contains values
	template<typename T>
	bool contains(std::vector<T> v, T x) {
		return std::find(v.begin(), v.end(), x) != v.end();
	}

	// obtain key list
	template<typename T>
	std::vector<std::string> keys(std::map<std::string, T> m) {
		std::vector<std::string> ret;

		// push each key from each pair
		for (auto const& element : m) {
			ret.push_back(element.first);
		}

		return ret;
	}

	// obtain combined key list
	template<typename T>
	std::vector<std::string> keys(std::map<std::string, T> m1, std::map<std::string, T> m2) {
		// get keys from each map
		std::vector<std::string> keySet1 = keys<T>(m1);
		std::vector<std::string> keySet2 = keys<T>(m2);

		// insert the second list into first
		keySet1.insert(keySet1.end(), keySet2.begin(), keySet2.end());

		// return result
		return keySet1;
	}

	// determine if character is number
	bool isNumber(char c, bool acceptDecimal, bool acceptNegative) {
		if (c >= '0' && c <= '9') {
			return true;
		}
		else if (acceptDecimal && c == '.') {
			return true;
		}
		else if (acceptNegative && c == '-') {
			return true;
		}

		return false;
	}

	// determine if entire string is number
	bool isNumber(const char* str) {
		return contains<std::string>(constantNames, str) ||
			contains<std::string>(keys<double>(variables), str) ||
			std::strcmp(str, ".") == 0 ||
			containsNumbers(str);
	}

	// determine if string only contains numerical characters
	bool containsNumbers(const char* str) {
		// try to prove wrong
		bool acceptDecimal = true;
		bool acceptNegative = true;
		for (char c : std::string(str)) {
			if (!isNumber(c, acceptDecimal, acceptNegative)) {
				return false;
			}

			if (c == '.') {
				acceptDecimal = false;
			} if (c == '-') {
				acceptNegative = false;
			}
		}

		return true;
	}

	// get numerical value of string
	double getNumericalVal(const char* str) {
		if (contains<std::string>(constantNames, str)) {
			return constants[str];
		}
		else if (contains<std::string>(keys<double>(variables), str)) {
			return variables[str];
		}
		else {
			return std::atof(str);
		}
	}

	// determine if string matches a function
	bool isFunction(std::string str) {
		return contains<std::string>(functionNames, str);
	}

	// determine if function is left associative
	bool isLeftAssociative(std::string str) {
		return binary_functions[str].left;
	}

	// get function precedence
	short getPrecedence(std::string str) {
		if (contains<std::string>(keys(binary_functions), str)) {
			return binary_functions[str].prec;
		}

		return 0;
	}

	// find element from list in the equation starting at index i
	std::string findElement(int i, const char* eqn, std::vector<std::string> list) {
		for (std::string item : list) {
			int n = (int)item.size();
			if (std::string(eqn).substr(i, n) == item) {
				return item;
			}
		}

		return "";
	}
} // namespace ShuntingYard
