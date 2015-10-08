#include "compile-time-strings.hpp"
#include <cassert>

struct EMPTY {
	constexpr static const char* str() {
		return "";
	}
};

struct Hello {
	constexpr static const char* str() {
		return "Hello ";
	}
};

struct World {
	constexpr static const char* str() {
		return "World";
	}
};

struct Exclamation {
	constexpr static const char* str() {
		return "!";
	}
};


int main() {
	using E = ctstring::ctstring<EMPTY>;
	using H = ctstring::ctstring<Hello>;
	using W = ctstring::ctstring<World>;
	using HW = ctstring::concat<H, W>;
	using X = ctstring::concat<H, W, ctstring::ctstring<Exclamation>>;
	using C0 = ctstring::concat<>;

	assert(E::size == 0);
	assert(E::string() == "");

	assert(H::size == 6);
	assert(H::string() == "Hello ");

	assert(W::size == 5);
	assert(W::string() == "World");

	assert(HW::size == 11);
	assert(HW::string() == "Hello World");

	assert(X::size == 12);
	assert(X::string() == "Hello World!");

	assert(C0::size == 0);
	assert(C0::string() == "");
}
