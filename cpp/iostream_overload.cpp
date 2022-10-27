/**
 * @file iostream_overload.cpp
 * @author César E Petersen (cesar.e.p@hotmail.com)
 * @brief example of iostream operator overloads
 * @version 0.1
 * @date 2022-10-27
 * 
 */


#include <iostream>

using namespace std;

struct ponto {
	float x, y, z;

	friend ostream& operator<<(ostream& os, const ponto& p) {
		os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
		return os;
	};

	friend istream& operator>>(istream& in, ponto& p) {
		in >> p.x >> p.y >> p.z;
		return in;
	};
};

int main() {
	ponto p{ 1., 2., 3.5 }, q;

	cout << "P=" << p << endl;

	cout << "digite ponto Q:\n";
	cin >> q;

	cout << "\nQ=" << q;

	return 0;
}
