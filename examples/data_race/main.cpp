#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>

// volatile const uint32_t * const ptr = 0xdeffffde;
int counter = 65535;
std::mutex mtx;

void worker(int delta) {
	for(auto i = 0U; i < 100; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		{
			std::lock_guard<std::mutex> _(mtx);
			//mtx.lock();
			counter += delta;
			//mtx.unlock();
		}
	}
}

int main() {
	auto t1 = std::thread{worker, 1};
	auto t2 = std::thread{worker, -1};

	t2.join();
	t1.join();

	std::cout << counter << " == 65535\n";
}
