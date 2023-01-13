#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

// CV

unsigned counter = 0;
std::condition_variable cv;
std::mutex mtx;

void worker() {
	std::cout << "Before barier\n";
	{
		std::unique_lock<std::mutex> lk(mtx);
		--counter;
		cv.notify_all();
		cv.wait(lk, []{ return counter == 0; });
	}
	std::cout << "After barier\n";
}

int main() {
	counter = 2;
	std::jthread th1(worker);
	std::jthread th2(worker);
	std::jthread th3(worker);
}
