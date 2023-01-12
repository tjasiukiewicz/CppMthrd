#include <iostream>

template<typename Fun, typename... Args>
void exec(Fun fun, Args&&... arg) {
	fun(std::forward<Args>(arg)...);
}

void foo(int&& a) {
	std::cout << "I'm foo(&&)\n";
}
int main() {
	int a = 12;
	exec(foo, std::move(a));
}
