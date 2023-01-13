#include <iostream>

float add_tax(float val) {
	return val * 1.23;
}

float add_shipping(float val) {
	return val + 0.2;
}

float add_salary(float val) {
	return val * 1.05;
}

namespace {

template<typename Func>
float decorate_impl(float price, Func func) {
	return func(price);
}

template<typename Func, typename... Funcs>
float decorate_impl(float price, Func func, Funcs... funcs) {
	float val = func(price);
	return decorate_impl(val, funcs...);
}

} // anonymous namespace

template<typename... Funcs>
float decorate(float price, Funcs... funcs) {
	return decorate_impl(price, funcs...);
}

struct my_decorator {
	my_decorator(float price_)
		: price{price_} {}

	template<typename Func>
	my_decorator & deco(Func func) {
		price = func(price);
		return *this;
	}

	float value() const {
		return price;
	}
private:
	float price;
};

int main() {
	float price = 1000.0;
	std::cout << add_shipping(add_tax(add_salary(price))) << '\n';
	std::cout << decorate(price, add_salary, add_tax, add_shipping) << '\n';
	std::cout << my_decorator(price)
		.deco(add_salary)
		.deco(add_tax)
		.deco(add_shipping).value() << '\n';
}
