#include <iostream>
#include <thread>
#include <future>

void work(std::promise<int>& pr) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    pr.set_value(42);
    std::cout << "I jadę dalej :-)\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Qniec\n";
}

int main() {
    std::promise<int> pr;
    auto ftr = pr.get_future();
    auto thr = std::thread(work, std::ref(pr));
    std::cout << ftr.get() << '\n';
    thr.join();
}
