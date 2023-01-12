#include <iostream>
#include <queue>
#include <chrono>
#include <functional>
#include <mutex>
#include <memory>
#include <cstddef>
#include <future>
#include <condition_variable>

template<typename T>
class thread_safe_queue {
    using glock_t = std::lock_guard<std::mutex>;
    using ulock_t = std::unique_lock<std::mutex>;
public:
    thread_safe_queue() = default;
    thread_safe_queue(const thread_safe_queue<T>& src) {
        if(&src == this) {
            return;
        }
        glock_t lck(src.mtx);
        data_queue = src.data_queue;
    }
    thread_safe_queue& operator=(const thread_safe_queue<T>& src) {
        if(&src != this) {
            glock_t lck1(src.mtx);
            glock_t lck2(mtx);
            data_queue = src.data_queue;
        }
        return *this;
    }
    thread_safe_queue(thread_safe_queue&&) = delete;
    thread_safe_queue& operator=(thread_safe_queue&&) = delete;

    void push(T value) {
        glock_t lck(mtx);
        data_queue.push(value);
        data_condition.notify_one();
    }

    std::shared_ptr<T> try_pop() {
        if(mtx.try_lock()) {
            glock_t lck(mtx, std::adopt_lock);
            if(!data_queue.empty()) {
                auto ptr_data{std::make_shared<T>(data_queue.front())};
                data_queue.pop();
                return ptr_data;
            }
        }
        return {};
    }

    std::shared_ptr<T> wait_and_pop() {
        ulock_t lck(mtx);
        data_condition.wait(lck, [this] { return !data_queue.empty(); });
        auto ptr_data{std::make_shared<T>(data_queue.front())};
        data_queue.pop();
        lck.unlock();
        return ptr_data;
    }

    bool try_pop(T& value) {
        // ... 
        return false;
    }

    bool wait_and_pop(T& value) {
        //...
        return false;
    }

    bool empty() const {
        glock_t lck(mtx);
        return data_queue.empty();
    }
    std::size_t size() const {
        glock_t lck(mtx);
        return data_queue.size();
    }
private:
    mutable std::mutex mtx;
    std::queue<T> data_queue;
    std::condition_variable data_condition;
};

int main() {
    thread_safe_queue<int> tsq;
    auto ftr1 = std::async(std::launch::async, [&tsq] {
            for(auto i = 0U; i < 10; ++i) {
                tsq.push(i + 10); 
            }
    });
    auto ftr2 = std::async(std::launch::async, [&tsq] {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            while(!tsq.empty()) {
                auto value = tsq.wait_and_pop();
                std::cout << (value ? *value: 0) << '\n';
            }
    });
    for(auto & ft: { &ftr1, &ftr2 }) {
        ft->get();
    }
}
