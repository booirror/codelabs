#include <iostream>

using namespace std;

class A {
public:
	virtual void print(int a);
	virtual void print(int a, int b);

	virtual void func(int a) { print(a); }
	virtual void func(int a, int b) { print(a, b); }
};

void A::print(int a)
{
	cout << "A: a=" << a << endl;
}

void A::print(int a, int b)
{
	cout << "A: a+b=" << a + b << endl;
}

class B :public A
{
public:
	virtual void print(int a) override;
	void func(const char* a);
};

void B::print(int a)
{
	cout << "B: a=" << a << endl;
}

void B::func(const char* a)
{
	cout << "B: func=" << a << endl;
}

int main()
{
	A * pa = new A();
	pa->print(3);
	pa->print(4, 5);

	A* pb = new B();
	pb->print(3);
	pb->print(4, 5);
	pb->func(6);
	pb->func(3, 3);

	B* b = new B();
	b->func("hello");



	return 0;
}

