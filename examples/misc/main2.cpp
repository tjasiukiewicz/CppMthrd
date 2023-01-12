#include <iostream>

template<typename T1, typename T2>
auto my_min(T1 a, T2 b) -> decltype(a + b) {
	return a < b ? a: b;
}

int main() {
	std::cout << my_min(12, 13.3) << '\n';
}
