#include <future>
#include <iostream>


std::packaged_task<void()> task_producer() {
    std::packaged_task<void()> tsk{[]{ std::cout << "Hi\n";}};
    return tsk;
}

int main() {
    task_producer()();

}
