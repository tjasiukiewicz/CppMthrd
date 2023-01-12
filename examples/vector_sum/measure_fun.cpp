#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <algorithm>
#include <vector>

template<unsigned Count = 100, typename Func, typename... Args>
void measure_time(const std::string& msg, Func func, Args&&... args) {
	std::vector<unsigned> times;
	times.reserve(Count);
	for (auto i = 0U; i < Count; ++i) {
    		auto start = std::chrono::high_resolution_clock::now();
		func(std::forward<Args>(args)...);
    		auto stop = std::chrono::high_resolution_clock::now();
		times.emplace_back(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count());
	}
	std::sort(times.begin(), times.end());
	// Median? Average ?
	std::cout << msg << ": " << times[Count / 2] << " ms.\n";
}
void dumb_function() {
    for(auto i = 0; i < 100; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main() {
	measure_time<10>("dumb function", dumb_function);
}
