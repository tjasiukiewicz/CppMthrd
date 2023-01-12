#include <iostream>

template<int Val>
struct Factorial {
	constexpr static int value = Factorial<Val - 1>::value * Val;
};

template<>
struct Factorial<0> {
	constexpr static int value = 1;
};

int main() {
	std::cout << Factorial<8>::value << '\n';
}
