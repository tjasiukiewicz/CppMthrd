#include <iostream>
#include <functional>

template<typename T>
using sink = std::function<void(const T&)>;

template<typename T>
using source = std::function<void(sink<T>)>;

source<char> consoleInput = [](sink<char> si) {
    int ichar;
    while((ichar = std::cin.get()) != EOF) {
        si(static_cast<char>(ichar));
    }
};

sink<char> consoleOutput = [](char c) {
    std::cout.put(c);
};

template<typename T>
void connect(source<T> so, sink<T> si) {
    so(si);
}

template<typename T>
source<T> operator>>(source<T> so, sink<T> si) {
    connect(so, si);
    return so;
}

int main() {
    //consoleInput(consoleOutput);
    //connect(consoleInput, consoleOutput);
    consoleInput >> consoleOutput;
}

