#include <iostream>
#include <functional>
#include <vector>
#include <stack>
#include <regex>
#include <cmath>

using namespace std;

enum MathFunctionType {
	UNARY = 0,
	BINARY = 1
};

class MathFunction {

	private:
		function<double(double, double)> evaluateBinary;
		function<double(double)> evaluateUnary;

	public:
		string name;
		MathFunctionType type;

		MathFunction(string name, function<double(double, double)> evaluate) {
			this->name = name;
			this->evaluateBinary = evaluate;
			this->type = MathFunctionType::BINARY;
		}

		MathFunction(string name, function<double(double)> evaluate) {
			this->name = name;
			this->evaluateUnary = evaluate;
			this->type = MathFunctionType::UNARY;
		}

		double evaluate(double x) {
			return this->evaluateUnary(x);
		}

		double evaluate(double x, double y) {
			return this->evaluateBinary(x, y);
		}
};

vector<MathFunction> mathFunctions = {
	MathFunction("abs", [](double x) -> double { return abs(x); }),
	MathFunction("acos", [](double x) -> double { return acos(x); }),
	MathFunction("acosh", [](double x) -> double { return acosh(x); }),
	MathFunction("asin", [](double x) -> double { return asin(x); }),
	MathFunction("asinh", [](double x) -> double { return asinh(x); }),
	MathFunction("atan", [](double x) -> double { return atan(x); }),
	MathFunction("atan2", [](double x, double y) -> double { return atan2(x, y); }),
	MathFunction("atanh", [](double x) -> double { return atanh(x); }),
	MathFunction("cbrt", [](double x) -> double { return cbrt(x); }),
	MathFunction("ceil", [](double x) -> double { return ceil(x); }),
	MathFunction("copysign", [](double x, double y) -> double { return copysign(x, y); }),
	MathFunction("cos", [](double x) -> double { return cos(x); }),
	MathFunction("cosh", [](double x) -> double { return cosh(x); }),
	MathFunction("exp", [](double x) -> double { return exp(x); }),
	MathFunction("exp2", [](double x) -> double { return exp2(x); }),
	MathFunction("expm1", [](double x) -> double { return expm1(x); }),
	MathFunction("fabs", [](double x) -> double { return fabs(x); }),
	MathFunction("fdim", [](double x, double y) -> double { return fdim(x, y); }),
	MathFunction("floor", [](double x) -> double { return floor(x); }),
	MathFunction("fmax", [](double x, double y) -> double { return fmax(x, y); }),
	MathFunction("fmin", [](double x, double y) -> double { return fmin(x, y); }),
	MathFunction("fmod", [](double x, double y) -> double { return fmod(x, y); }),
	MathFunction("hypot", [](double x, double y) -> double { return hypot(x, y); }),
	MathFunction("ilogb", [](double x) -> double { return ilogb(x); }),
	MathFunction("ldexp", [](double x, double y) -> double { return ldexp(x, y); }),
	MathFunction("llrint", [](double x) -> double { return llrint(x); }),
	MathFunction("llround", [](double x) -> double { return llround(x); }),
	MathFunction("log", [](double x) -> double { return log(x); }),
	MathFunction("log10", [](double x) -> double { return log10(x); }),
	MathFunction("log1p", [](double x) -> double { return log1p(x); }),
	MathFunction("log2", [](double x) -> double { return log2(x); }),
	MathFunction("logb", [](double x) -> double { return logb(x); }),
	MathFunction("lrint", [](double x) -> double { return lrint(x); }),
	MathFunction("lround", [](double x) -> double { return lround(x); }),
	MathFunction("nearbyint", [](double x) -> double { return nearbyint(x); }),
	MathFunction("nextafter", [](double x, double y) -> double { return nextafter(x, y); }),
	MathFunction("nexttoward", [](double x, double y) -> double { return nexttoward(x, y); }),
	MathFunction("pow", [](double x, double y) -> double { return pow(x, y); }),
	MathFunction("remainder", [](double x, double y) -> double { return remainder(x, y); }),
	MathFunction("rint", [](double x) -> double { return rint(x); }),
	MathFunction("round", [](double x) -> double { return round(x); }),
	MathFunction("scalbln", [](double x, double y) -> double { return scalbln(x, y); }),
	MathFunction("scalbn", [](double x, double y) -> double { return scalbn(x, y); }),
	MathFunction("sin", [](double x) -> double { return sin(x); }),
	MathFunction("sinh", [](double x) -> double { return sinh(x); }),
	MathFunction("sqrt", [](double x) -> double { return sqrt(x); }),
	MathFunction("tan", [](double x) -> double { return tan(x); }),
	MathFunction("tanh", [](double x) -> double { return tanh(x); }),
	MathFunction("trunc", [](double x) -> double { return trunc(x); })
};

vector<string> mathFunctionNames = ([]() -> vector<string> {
	vector<string> names = {};
	for (MathFunction fn : mathFunctions) {
		names.push_back(fn.name);
	}
	return names;
})();

enum ExpressionTokenType {
	OPERAND = 0,
	OPERATOR = 1,
	FUNCTION = 2,
	LEFT_PAREN = 3,
	RIGHT_PAREN = 4
};

enum ExpressionOperatorAssociativity {
	LEFT = 0,
	RIGHT = 1
};

class ExpressionToken {
	public:
		string raw;
		ExpressionToken(string rawStr) {
			this->raw = rawStr;
		};
		
		ExpressionTokenType getType() {
			if (
				this->raw == "^" ||
				this->raw == "*" ||
				this->raw == "/" ||
				this->raw == "+" ||
				this->raw == "-"
			) {
				return OPERATOR;
			}

			if (this->raw == "(") {
				return LEFT_PAREN;
			}

			if (this->raw == ")") {
				return RIGHT_PAREN;
			}

			if (
				std::find(
					mathFunctionNames.begin(),
					mathFunctionNames.end(),
					this->raw
				) != mathFunctionNames.end()
			) {
				return FUNCTION;
			}

			return OPERAND;
		}

		ExpressionOperatorAssociativity getAssociativity() {
			if (
				this->raw == "*" ||
				this->raw == "/" ||
				this->raw == "+" ||
				this->raw == "-"
			) {
				return LEFT;
			}

			if (this->raw == "^" ) {
				return RIGHT;
			}

			return RIGHT;
		}

		int getPrecedence() {
			if (this->raw == "^") return 4;
			if (this->raw == "*") return 3;
			if (this->raw == "/") return 3;
			if (this->raw == "+") return 2;
			if (this->raw == "-") return 2;

			return 1;
		}
};

class Expression {
	public:
		vector<ExpressionToken> tokens = {};
		Expression() {}
		Expression(vector<ExpressionToken> tokens) {
			this->tokens = tokens;
		}


		static Expression fromInfixString(string str) {

			str = regex_replace(str, regex(","), " ");
			str = regex_replace(str, regex("([+-/*()\\^])"), " $1 ");
			str = regex_replace(str, regex("\\s{2,}"), " ");
			str = regex_replace(str, regex("^\\s"), "");
			str = regex_replace(str, regex("\\s$"), "");

			vector<ExpressionToken> tokens = {};

			int _i;
			int i = str.find(" ");
			while (i != string::npos) {
				string token = regex_replace(
					str.substr(0, i),
					regex("\\s*"),
					""
				);
				if (token != "") tokens.push_back(token);

				str.erase(0, i + 1);
				_i = i;
				i = str.find(" ");
			}

			string token = regex_replace(
				str.substr(0, _i),
				regex("\\s*"),
				""
			);
			if (token != "") tokens.push_back(token);

			return Expression(tokens).toPostfix();
		}

		double evaluateForX(double x) {
			stack<double> operands = {};

			for (ExpressionToken token : this->tokens) {
				if (token.getType() == ExpressionTokenType::OPERATOR) {
					double op2 = operands.top();
					operands.pop();
					double op1 = operands.top();
					operands.pop();
					
					if (token.raw == "^") operands.push(pow(op1, op2));
					if (token.raw == "*") operands.push(op1 * op2);
					if (token.raw == "/") operands.push(op1 / op2);
					if (token.raw == "+") operands.push(op1 + op2);
					if (token.raw == "-") operands.push(op1 - op2);
				}
				
				else if (token.getType() == ExpressionTokenType::FUNCTION) {
					for (MathFunction fn : mathFunctions) {
						if (fn.name == token.raw) {
							if (fn.type == MathFunctionType::UNARY) {
								double op1 = operands.top();
								operands.pop();
								operands.push(fn.evaluate(op1));
							}

							if (fn.type == MathFunctionType::BINARY) {
								double op2 = operands.top();
								operands.pop();
								double op1 = operands.top();
								operands.pop();
								operands.push(fn.evaluate(op1,op2));
							}

							break;
						}
					}
				}

				else {
					double val;
					if (token.raw == "x") val = x;
					else if (token.raw == "pi") val = 3.14159;
					else if (token.raw == "e") val = 2.71828;
					else val = stod(token.raw);

					operands.push(val);
				}
			};

			return operands.top();
		}

	private:
		Expression toPostfix() {
			stack<ExpressionToken> operatorStack = {};
			vector<ExpressionToken> postfixQueue = {};
			vector<ExpressionToken> infixTokens = this->tokens;

			
			for (ExpressionToken token : infixTokens) {
				if (token.getType() == ExpressionTokenType::OPERAND) {
					postfixQueue.push_back(token);
				}

				if (token.getType() == ExpressionTokenType::FUNCTION) {
					operatorStack.push(token);
				}

				if (token.getType() == ExpressionTokenType::OPERATOR) {
						
					if (operatorStack.empty()) {
						operatorStack.push(token);
					} else {
						while (
							(operatorStack.top().getType() == ExpressionTokenType::FUNCTION) ||
							(operatorStack.top().getPrecedence() > token.getPrecedence()) ||
							(
								operatorStack.top().getPrecedence() == token.getPrecedence()
								&& operatorStack.top().getAssociativity() == ExpressionOperatorAssociativity::LEFT
							) &&
							(operatorStack.top().getType() != ExpressionTokenType::LEFT_PAREN)
						) {
							postfixQueue.push_back(operatorStack.top());
							operatorStack.pop();

							if (operatorStack.empty()) break;
						}

						operatorStack.push(token);
					}
				}

				if (token.getType() == ExpressionTokenType::LEFT_PAREN) {
					operatorStack.push(token);
				}

				if (token.getType() == ExpressionTokenType::RIGHT_PAREN) {
					ExpressionToken topOperator = operatorStack.top();

					while (operatorStack.top().getType() != ExpressionTokenType::LEFT_PAREN) {
						postfixQueue.push_back(operatorStack.top());
						operatorStack.pop();
					}

					if (operatorStack.top().getType() == ExpressionTokenType::LEFT_PAREN) {
						operatorStack.pop();
					};
				}
			}

			while (!operatorStack.empty()) {
				postfixQueue.push_back(operatorStack.top());
				operatorStack.pop();
			}

			return Expression(postfixQueue);
		}
};

class Marser {
	public:
		Expression expr;
		Marser(string exprStr) {
			this->expr = Expression::fromInfixString(exprStr);
		}

		double eval(double x) {
			return this->expr.evaluateForX(x);
		}
};