#include <iostream>

template <size_t t, class T>
void print(T m)
{
	std::cout << t << std::endl;
}

int const_main()
{

	const int a = 3;
	int b = 4;
	constexpr const int c = b;

	print<a, int>(0);

	print<c, int>(0);

	return 0;
}