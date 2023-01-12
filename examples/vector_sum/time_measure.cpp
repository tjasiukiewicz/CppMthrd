#include <iostream>
#include <chrono>
#include <thread>

void dumb_function() {
    for(auto i = 0; i < 100; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    dumb_function();
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Funkcja wykonywała się: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() 
        << " ms.\n";
    
}
