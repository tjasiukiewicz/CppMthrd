#include <shared_mutex>
#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include <chrono>


std::shared_mutex smtx;
std::mutex stream_mutex;
std::queue<int> que;

void reader() {
	for(auto i = 0U; i < 5; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		{
			smtx.lock_shared();
			{
				std::lock_guard<std::mutex> _(stream_mutex);
				std::cout << "reader: " << que.front() << '\n';
			}
			que.pop();
			smtx.unlock_shared();
		}
	}
}

void writer() {
	for(auto i = 0U; i < 10; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		{
			std::lock_guard<std::shared_mutex> _(smtx);
			que.push(i);
			{
				std::lock_guard<std::mutex> _(stream_mutex);
				std::cout << "writer: " << i << '\n';
			}
		}
	}
}
int main() {
	std::jthread wt(writer);
	std::jthread rt1(reader);
	std::jthread rt2(reader);
}
