#include <cassert>
#include <string>
#include "typedecl/typedecl.hpp"

/*
 * Implemented following the article "C++ Rvalue References Explained" by Thomas Becker.
 * http://thbecker.net/articles/rvalue_references/section_01.html
 */


using namespace std;

class X {
public:
	X() = default;

	string overloaded_constructor_or_assignment_operator(const X&) {
		return "lvalue";
	}

	string overloaded_constructor_or_assignment_operator(X&&) {
		return "rvalue";
	}
};


string overloaded(X&) {
	return "lvalue";
}

string overloaded(X&&) {
	return "rvalue";
}


X returns_x() {
	return {};
}
#define rvalue returns_x()


X&& returns_rvalue_ref_to_x() {
	X&& x = X();
	return static_cast<X&&>(x);
}
#define rvalue_without_a_name returns_rvalue_ref_to_x()


DEFINE_TYPEDECL(X);
template <typename T>
string template_rvalue_ref_arg_deduction(T&& arg) {
	using ArgType = decltype(arg);
	return "deduced template argument is " + typedecl<T>()
	      + "; function argument type is " + typedecl<ArgType>();
}


int main() {
	X lvalue;
	assert(overloaded(lvalue) == "lvalue");
	assert(overloaded(rvalue) == "rvalue");

	X x;
	assert(x.overloaded_constructor_or_assignment_operator(lvalue) == "lvalue");
	assert(x.overloaded_constructor_or_assignment_operator(rvalue) == "rvalue");

	X&& rvalue_with_a_name = rvalue;
	assert(x.overloaded_constructor_or_assignment_operator(rvalue_with_a_name)    == "lvalue");
	assert(x.overloaded_constructor_or_assignment_operator(rvalue_without_a_name) == "rvalue");

	assert(template_rvalue_ref_arg_deduction(lvalue) == "deduced template argument is X&; function argument type is X&");
	assert(template_rvalue_ref_arg_deduction(rvalue) == "deduced template argument is X; function argument type is X&&");
}
