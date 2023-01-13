#include <iostream>
#include <functional>
#include <cctype>

template<typename T>
using sink = std::function<void(const T&)>;

template<typename T>
using source = std::function<void(sink<T>)>;

template<typename T1, typename T2>
using transform = std::function<void(sink<T2>, T1)>;

source<char> consoleInput = [](sink<char> si) {
    int ichar;
    while((ichar = std::cin.get()) != EOF) {
        si(static_cast<char>(ichar));
    }
};

sink<char> consoleOutput = [](char c) {
    std::cout.put(c);
};

transform<char, char> toUpperTransform = [](sink<char> si, char c) {
    char ichar;
    ichar = toupper(c);
    si(ichar);
};

template<typename T>
void connect(source<T> so, sink<T> si) {
    so(si);
}

template<typename T1, typename T2>
source<T2> applyToSink(sink<T1>& si, transform<T1, T2>& tr) {
    return [=](const T2& c) {
        tr(si, c);
    };
}

template<typename T1, typename T2>
source<T2> applyToSource(source<T1>& so, transform<T1, T2>& tr) {
    return [&](sink<T2> si) {
        so([=](const T2& c){ tr(si, c); });
    };
}

template<typename T>
source<T> operator>>(source<T> so, sink<T> si) {
    connect(so, si);
    return so;
}

template<typename T1, typename T2>
source<T2> operator>>(transform<T1, T2>& tr, sink<T2>& si) {
    return applyToSink(si, tr);
}

template<typename T1, typename T2>
source<T2> operator>>(source<T1>& so, transform<T1, T2>& tr) {
    return applyToSource(so, tr);
}

int main() {
    //consoleInput(consoleOutput);
    //connect(consoleInput, consoleOutput);
    //consoleInput >> consoleOutput;
    consoleInput >> toUpperTransform >> consoleOutput;
}

