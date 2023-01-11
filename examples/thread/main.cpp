#include <iostream>
#include <chrono>
#include <thread>

void worker() {
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "I'm worker()\n";
}

void worker1(int val) {
	std::cout << "I'm worker(" << val << ")\n";
}

struct X {
	void operator()() {
		std::cout << "X operator()\n";
	}
	void run(int val) {
		std::cout << "X run(" << val << ")\n";
	}
};

void work_ref(int& val) {
	std::cout << "I'm work_ref(" << val << ")\n";
}
int main() {
	X x;
	int a = 12;
	std::thread thr1{worker};
	std::thread thr2{worker1, 42};
	std::thread thr3{[](float val){ std::cout << "I'm lambda("
		<< val << ")\n";
	}, 82};
	std::thread thr4{x};
	std::thread thr5{&X::run, &x, 78};
	std::thread thr6;
	std::thread thr7{work_ref, std::ref(a)};
	thr6 = std::move(thr7);

	for(auto & t: {&thr1, &thr2, &thr3, &thr4, &thr5, &thr6, &thr7}) {
		if(t->joinable()) {
			t->join();
		}
	}
}

