#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

std::queue<int> que;
std::mutex mtx;
std::condition_variable cv;

void reader() {
	for(auto i = 0U; i < 5; ++i) {
		std::unique_lock<std::mutex> lk(mtx);
		cv.wait(lk, []{ return !que.empty(); });
		auto val = que.front();
		que.pop();
		std::cout << "reader: " << val << '\n';
	}
}

void writer() {
	for(auto i = 0U; i < 10; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		que.push(i + 10);
		cv.notify_one();
	}
}

int main() {
	std::thread th1{writer};
	std::thread th2{reader};
	std::thread th3{reader};

	th3.join();
	th2.join();
	th1.join();
}
