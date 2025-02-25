#include <iostream>
#include "../marser.h"

int main(int argc, char* argv[]) {

	cout << "y(x) = ";
	char _str[100];
	fgets(_str, 100, stdin);
	string expr = string(_str);

	Marser y = Marser(expr);

	while (true) {
		double x;
		cout << "x = ";
		cin >> x;
		cout << "y = " << y.eval(x) << endl << endl;
	}
}