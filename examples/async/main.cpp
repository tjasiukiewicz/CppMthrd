#include <iostream>
#include <iomanip>
#include <future>

int main() {
	std::future<int> ftr1 = std::async(std::launch::async, [](int a) {
			std::cout << a << '\n';
			return a * 2;
	}, 12);
	std::cout << ftr1.get() << '\n';
	std::cout << std::boolalpha << ftr1.valid() << '\n';
	//ftr1.get();
	ftr1 = std::async(std::launch::deferred, [](int a) {
			std::cout << a << '\n';
			return a * 2;
	}, 12);
	std::cout << ftr1.get() << '\n';
	// auto ftr2 = std::async(std::launch::deferred | std::launch::sync, fun)
	std::shared_future<int> ftr2 = std::async(std::launch::deferred, [](int a) {
			std::cout << a << '\n';
			return a * 2;
	}, 12);
	std::cout << ftr2.get() << '\n';
}
