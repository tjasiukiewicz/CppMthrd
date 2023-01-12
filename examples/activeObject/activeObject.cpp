#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <mutex>
#include <functional>

enum class Priority {
    Low,
    Normal,
    High
};

struct Work {
    Priority priority;
    std::function<void()> job;
};

struct Comparator {
    bool operator()(const Work& w1, const Work& w2) const {
        return static_cast<int>(w1.priority) < static_cast<int>(w2.priority);
    }
};

class ActivePrioObject {
public:
    ActivePrioObject(): doStop{false}, jobs_queue{} {
        thr = std::thread(&ActivePrioObject::work, this);
    }
    void exec(Priority prio, std::function<void()> func) {
    	// ...
    }
    ~ActivePrioObject() {
    	// ...
    }
private:
    void work() {
    	// ...
    }
    bool doStop;
    std::thread thr;
    std::priority_queue<Work, std::vector<Work>, Comparator> jobs_queue;
    mutable std::mutex mtx;
};

void fooHigh() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "High foo\n";
}
void fooNormal() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "Normal foo\n";
}

void fooLow() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "Low foo\n";
}

int main() {
    ActivePrioObject prioO;
    prioO.exec(Priority::Normal, fooNormal);
    prioO.exec(Priority::High, fooHigh);
    prioO.exec(Priority::Normal, fooNormal);
    prioO.exec(Priority::Normal, fooNormal);
    prioO.exec(Priority::Normal, fooNormal);
    prioO.exec(Priority::Normal, fooNormal);
    prioO.exec(Priority::Normal, fooNormal);
    prioO.exec(Priority::Low, fooLow);
    prioO.exec(Priority::Normal, fooNormal);
    prioO.exec(Priority::Normal, fooNormal);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
