#include <iostream>
#include <vector>
#include "AllClasses.h"

enum AMMO{
	BIG,
	MEDIUM,
	SMALL
};

class A{
private:
	int a = 1;
protected:
public:
	virtual void f() = 0;
};

class B : public A{
	int a[100];
	int b;
public:
	void f() override{
		b = 1;
		std::cout << "zxc " << b << std::endl;
	}
};

int main() {
//	class B b;
//	b.f();
//
//	class B *ap = new class B[10], *ar = ap;
//	ap->f();
//
//	int a = BIG;
//	std::cout << a << std::endl;
//	ErrorCodes a = ERROR;
//	std::cout << a << std::endl;
	int b = 5;
	float a = 0.7;
	int &&c = 1;
	c++;
	std::cout << a << ' ' << (int) a << std::endl;
	std::cout << b << ' ' << (int)((float)b*a) << std::endl;
	std::cout << c << std::endl;
	
}
