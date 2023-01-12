#include <iostream>
#include <future>
#include <functional>
#include <vector>

int main() {
    using task_t = std::packaged_task<int(int)>;
    task_t task1{[](int a) { return a + 10; }};
    auto ftr1 = task1.get_future();

    task1(5);

    std::cout << ftr1.get() << '\n';

    task_t task2{[](int a) { return a + 10; }};
    auto ftr2 = task2.get_future();

    std::thread thr1{std::move(task2), 50};
    thr1.detach();
    std::cout << ftr2.get() << '\n';

    std::vector<task_t> task_container;
    task_container.emplace_back(task_t([](int a) { return a + 20; }));
    task_container.emplace_back(task_t([](int a) { return a + 40; }));

    size_t choice = 0;
    auto ftr = task_container[choice].get_future();
    std::thread(std::move(task_container[choice]), 60).detach();
    std::cout << ftr.get() << '\n';

    ftr = task_container[1].get_future();
    task_container[1](67);
    std::cout << ftr.get() << '\n';

}
