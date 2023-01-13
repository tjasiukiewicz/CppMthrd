#include <iostream>
template<class T> class Singleton {
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);
protected:
	Singleton() {}
	virtual ~Singleton() {}
public:
	static T& getInstance() {
		static T _instance;
		return _instance;
	}
};

class IsSingleton: public Singleton<IsSingleton> {
	int value;
protected:
	friend class Singleton<IsSingleton>;
	IsSingleton() { value = 224; }
public:
	int getValue() const { return value; }
	void setValue(int a) {
		value = a;
	}
};

int main(int argc, char* argv[])
{
	IsSingleton& s1 = IsSingleton::getInstance();
	IsSingleton& s2 = IsSingleton::getInstance();
	std::cout << s1.getValue() << "\n";
	s1.setValue(211);
	std::cout << s1.getValue() << "\n";
	std::cout << s2.getValue() << "\n";
	return 0;
}
