#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <string>

struct Seaman {
	Seaman()
		: position{way_width / 2}, stop{false} {
		thr_left = std::thread{&Seaman::walk, this, -1};
		thr_right = std::thread{&Seaman::walk, this, 1};
		show_way();
	}
	~Seaman() {
		stop = true;
		thr_left.join();
		thr_right.join();
	}
	void show_way() const {
		while(!stop) {
			std::cout << "|" << std::string(position, '-') << '*' 
				<< std::string(way_width - position, '-') << "|\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
	void walk(int delta) {
		while(!stop) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			position += delta;
			if ((position == 0) or (position == way_width)) {
				stop = true;
				break;
			}
		}
	}
private:
	static constexpr unsigned way_width = 11;
	unsigned position;
	mutable std::mutex mtx;
	std::thread thr_left;
	std::thread thr_right;
	bool stop;
};

int main() {
	Seaman seaman;
}
