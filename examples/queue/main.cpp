#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <optional>



struct TQueue {
	TQueue()
		: is_stop{false} {
	}
	void push(int val) {
		std::lock_guard<std::mutex> lk(mtx);
		data.push(val);
		cv.notify_one();
	}
	std::optional<int> pop() {
		std::unique_lock<std::mutex> lk(mtx);
		cv.wait(lk, [this]{ return is_stop or !data.empty();});
		if (is_stop) {
			return {};
		}
		int val = data.front();
		data.pop();
		return {val};
	}
	std::optional<int> async_pop() {
		std::lock_guard<std::mutex> lk(mtx);
		if ( !data.empty()) {
			int val = data.front();
			data.pop();
			return {val};
		}
		return {};
	}
	/*
	std::shared_ptr<int> pop() {
	}
	bool pop(int& box) {
	}
	*/
	void stop() {
		is_stop = true;
		cv.notify_all();
	}
private:
	std::condition_variable cv;
	std::mutex mtx;
	std::queue<int> data;
	bool is_stop;
};

void reader(TQueue& q) {
	for (auto i = 0U; i < 6; ++i) {
		auto v = q.pop();
		if(v) {
			std::cout << "reader: " << *v << '\n';
		} else {
			std::cout << "reader: no value!\n";
		}
	}
}

void writer(TQueue& q) {
	for (auto i = 0U; i < 10; ++i) {
		q.push(i + 10);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

int main() {
	TQueue q;
	std::thread th1{writer, std::ref(q)};
	std::thread th2{reader, std::ref(q)};
	std::thread th3{reader, std::ref(q)};
	std::this_thread::sleep_for(std::chrono::seconds(3));
	q.stop();
	th3.join();
	th2.join();
	th1.join();
}
