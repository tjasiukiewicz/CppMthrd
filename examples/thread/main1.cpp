#include <iostream>
#include <thread>
#include <chrono>

template<typename Fun, typename... Args>
void executor(std::chrono::milliseconds ms, Fun fun, Args&&... args) {
	auto now = std::chrono::steady_clock::now();
	auto next_run = now + ms;
	for (;;) {
		fun(std::forward<Args>(args)...);
		std::this_thread::sleep_until(next_run);
		next_run += ms;
	}
}

void simple_worker(int a) {
	std::cout << "I'm simple_worker(" << a << ")\n";
}

void worker() {
	auto now = std::chrono::steady_clock::now();
	auto next_run = now + std::chrono::milliseconds(500);
	for (auto i = 0U; i < 10; ++i) {
		std::cout << "Siema!\n";
		std::this_thread::sleep_until(next_run);
		next_run += std::chrono::milliseconds(500);
	}
}

int main() {
	/*
	auto thr1 = std::thread{worker};
	thr1.join();
	*/
	executor(std::chrono::milliseconds(500), simple_worker, 42);
}
