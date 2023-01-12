#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
// 1. Stwórz std::vector<int> z 1000 elementów losowych [0, 100]
// 2. W wątkach policz sumę elementów.
// 3. Zmierz czas wykonania w stosunku do wersji "serialnej".
//
// Wnioski:
// Dlaczego rozwiązanie z wątkami nie jest szybkie dla małej ilości danych?
// 1. Koszt powołania do życia wątku.
// 2. Koszty wykonania serialnego części reduce.
// 3. Wpływ false sharing.
//
// Jakie mogą być usprawnienia?
// 1. ....
//

constexpr static size_t vector_size = 40'000;
using container_t = std::vector<int>;

void data_generate(container_t& data) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);
    std::generate(data.begin(), data.end(),
        [&] {
            return dis(gen);
    });
}

long int serial_calculate(container_t::const_iterator beg, container_t::const_iterator end) {
    return std::accumulate(beg, end, 0);
}

long int parallel_calculate(const container_t& data) {
    auto thr_counter = std::thread::hardware_concurrency();
    thr_counter = (thr_counter == 0? 2: thr_counter);
    auto offset = vector_size / thr_counter;
    std::vector<std::future<long int>> results(thr_counter);
    // map
    for(auto i = 0U; i < results.size(); ++i) {
        results[i] = std::move(std::async(std::launch::async,
            [](container_t::const_iterator beg, container_t::const_iterator end) {
                return serial_calculate(beg, end);
        }, data.cbegin() + (i * offset), data.cbegin() + ((i + 1) * offset)));
    }
    // reduce
    return std::accumulate(results.begin(), results.end(), 0L,
            [](long int sum, decltype(results)::value_type& ftr) {
                return sum += ftr.get();
    });
}

int main() {
    container_t vec(vector_size);
    data_generate(vec);

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << serial_calculate(vec.cbegin(), vec.cend()) << ": ";
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Wykonanie serial: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << '\n';

    start = std::chrono::high_resolution_clock::now();
    std::cout << parallel_calculate(vec) << ": ";
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Wykonanie par: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << '\n';
}
