#include <iostream>
#include "../marser.h"

int main(int argc, char* argv[]) {
	char _str[100];
	fgets(_str, 100, stdin);
	string str = string(_str);

	Expression expr = Expression::fromInfixString(str);
	for (ExpressionToken token : expr.tokens) {
		std::cout << "[" << token.raw << "] ";
	}
}