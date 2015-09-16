#include <iostream>

using namespace std;


struct Stats;
Stats* stats = nullptr;

struct Stats {
	Stats(const char* class_, const char* function) {
		cout << "=== " << class_ << ' ' << function << endl;
		stats = this;
	}
	~Stats() {
		stats = nullptr;
		cout << endl;
	}
	void defaultConstructor() { cout << "Default constructor" << endl; }
	void copyConstructor() { cout << "Copy constructor" << endl; }
	void moveConstructor() { cout << "Move constructor" << endl; }
	void copyAssignment() { cout << "Copy assignment" << endl; }
	void moveAssignment() { cout << "Move assignment" << endl; }
	void unifiedAssignment() { cout << "Unified assignment" << endl; }
};


template <typename T>
void assignFromVariable() {
	Stats s(T::NAME, "assignFromVariable");
	T v1, v2;
	v1 = v2;
}


template <typename T>
T function(T& object) {
	return true ? object : object;
}

template <typename T>
void assignFromTemp() {
	Stats s(T::NAME, "assignFromTemp");
	T v1, v2;
	v1 = function(v2);
}


struct Separated {
	Separated() { stats->defaultConstructor(); }
	Separated(const Separated&) { stats->copyConstructor(); }
	Separated(Separated&&) { stats->moveConstructor(); }
	
	Separated& operator=(const Separated&) { stats->copyAssignment(); return *this; }
	Separated& operator=(Separated&&) { stats->moveAssignment(); return *this; }
	
	static const char* const NAME;
};
const char* const Separated::NAME = "Separated";

struct Unified {
	Unified() { stats->defaultConstructor(); }
	Unified(const Unified&) { stats->copyConstructor(); }
	Unified(Unified&&) { stats->moveConstructor(); }
	
	Unified& operator=(Unified) { stats->unifiedAssignment(); return *this; }
	
	static const char* const NAME;
};
const char* const Unified::NAME = "Unified";


int main() {
	assignFromVariable<Separated>();
	assignFromTemp<Separated>();
	assignFromVariable<Unified>();
	assignFromTemp<Unified>();
}
