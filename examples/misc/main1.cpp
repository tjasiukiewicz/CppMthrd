#include <iostream>

template<typename... Args>
void num_args(Args... args) {
	std::cout << "Num args: " << sizeof...(args) << '\n';
}

template<typename Arg>
void print_args(Arg arg) {
	std::cout << arg << '\n';
}

template<typename Arg, typename... Args>
void print_args(Arg arg, Args... args) {
	std::cout << arg << ' ';
	print_args(args...);
}

int main() {
	print_args(1, 2, "ala", 32.3);

}
