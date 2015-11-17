#include <cassert>
#include <iostream>
#include <string>
#include <utility>
using namespace std;


string red(const string& s)     { return "\e[31m" + s + "\e[0m"; }
string green(const string& s)   { return "\e[32m" + s + "\e[0m"; }
string yellow(const string& s)  { return "\e[33m" + s + "\e[0m"; }
string blue(const string& s)    { return "\e[34m" + s + "\e[0m"; }
string magenta(const string& s) { return "\e[35m" + s + "\e[0m"; }
string cyan(const string& s)    { return "\e[36m" + s + "\e[0m"; }


struct Result {
	const string deducedTemplateArg;
	const string rvalueRefSolvedAs;
	const string overloadCalled;

	Result(const string& ded, const string& as, const string& oc)
		: deducedTemplateArg(ded), rvalueRefSolvedAs(as), overloadCalled(oc)
	{}
};
bool operator==(const Result& lhs, const Result& rhs) {
	return lhs.overloadCalled == rhs.overloadCalled
	    && lhs.deducedTemplateArg == rhs.deducedTemplateArg
	    && lhs.rvalueRefSolvedAs == rhs.rvalueRefSolvedAs;
}


class C {
public:
	const string name;
	C(const string& name) : name(name) {}
};


template <typename T> struct type;
template <>           struct type<C>       { static string name() { return "C"; } };
template <typename T> struct type<T&>      { static string name() { return type<T>::name() + "&"; } };
template <typename T> struct type<T&&>     { static string name() { return type<T>::name() + "&&"; } };
template <typename T> struct type<const T> { static string name() { return "const " + type<T>::name(); } };


string overloadedFunction(const C&) { return "const C&"; }
string overloadedFunction(C&)       { return "C&"; }

template <typename T>
Result forwardingFunction(T&& arg) {
	string deducedTemplateArg = type<T>::name();
	string rvalueRefSolvedAs = type<T&&>::name();

	string overloadCalled = overloadedFunction(forward<T>(arg));

	cout << "Calling forwardingFunction(" << cyan("T") << yellow("&&") << " arg) with " << blue(arg.name) << endl;
	cout << "\tDeduced " << cyan("T") << " = " << cyan(deducedTemplateArg) << endl;
	cout << "\tType of arg is " << cyan("T") << yellow("&&") << " = " << cyan(deducedTemplateArg) << " " << yellow("&&") << " = " << green(rvalueRefSolvedAs) << endl;
	cout << "\tForwarded to: overloadedFunction(" << red(overloadCalled) << ")" << endl;
	cout << endl;
	return {deducedTemplateArg, rvalueRefSolvedAs, overloadCalled};
}


enum Category { LVALUE, RVALUE };

template <typename T, Category CAT>
T returns() {
	static C c = type<T>::name() + " " + (CAT == LVALUE ? "lvalue" : "rvalue");
	return static_cast<T>(c);
}

int main() {
	C                    lvalue = returns<C, LVALUE>();
	C&              lref_lvalue = returns<C&, LVALUE>();
	C&&             rref_lvalue = returns<C&&, LVALUE>();
	const C        const_lvalue = returns<const C, LVALUE>();
	const C&  const_lref_lvalue = returns<const C&, LVALUE>();
	const C&& const_rref_lvalue = returns<const C&&, LVALUE>();

	assert(forwardingFunction(lvalue)            == Result(      "C&",       "C&",       "C&"));
	assert(forwardingFunction(lref_lvalue)       == Result(      "C&",       "C&",       "C&"));
	assert(forwardingFunction(rref_lvalue)       == Result(      "C&",       "C&",       "C&"));
	assert(forwardingFunction(const_lvalue)      == Result("const C&", "const C&", "const C&"));
	assert(forwardingFunction(const_lref_lvalue) == Result("const C&", "const C&", "const C&"));
	assert(forwardingFunction(const_rref_lvalue) == Result("const C&", "const C&", "const C&"));


	#define            rvalue returns<C, RVALUE>()
	#define       lref_rvalue returns<C&, RVALUE>()
	#define       rref_rvalue returns<C&&, RVALUE>()
	#define      const_rvalue returns<const C, RVALUE>()
	#define const_lref_rvalue returns<const C&, RVALUE>()
	#define const_rref_rvalue returns<const C&&, RVALUE>()

	assert(forwardingFunction(           rvalue) == Result(      "C" ,       "C&&", "const C&"));
	assert(forwardingFunction(      lref_rvalue) == Result(      "C&",       "C&" ,       "C&"));
	assert(forwardingFunction(      rref_rvalue) == Result(      "C" ,       "C&&", "const C&"));
	assert(forwardingFunction(     const_rvalue) == Result("const C" , "const C&&", "const C&"));
	assert(forwardingFunction(const_lref_rvalue) == Result("const C&", "const C&" , "const C&"));
	assert(forwardingFunction(const_rref_rvalue) == Result("const C" , "const C&&", "const C&"));

	cout << "OK" << endl;
}
