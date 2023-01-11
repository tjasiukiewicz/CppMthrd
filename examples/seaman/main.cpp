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
		unsigned my_pos;
		while(!stop) {
			{
				std::lock_guard<std::mutex> _(mtx);
				my_pos = position;
			}
			show_road(my_pos);
			std::this_thread::sleep_for(std::chrono::milliseconds(80));
		}
		show_road(my_pos);
	}
	void walk(int delta) {
		while(!stop) {
			unsigned my_pos;
			std::this_thread::sleep_for(std::chrono::milliseconds(
				100 - 10 * delta));
			{
				std::lock_guard<std::mutex> _(mtx);
				position += delta;
				my_pos = position;
			}
			if ((my_pos < 1) or (my_pos == way_width)) {
				stop = true;
				break;
			}
		}
	}
private:
	void show_road(unsigned pos) const {
		std::cout << "|" << std::string(pos - 1, '-') << '*' 
			<< std::string(way_width - pos - 1, '-') << "|\n";
	}
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
