#include <cassert>
#include <iostream>
#include <string>
#include <utility>
using namespace std;

struct Result {
	enum Method { VAL, REF };

	const string deducedTemplateArg;
	const string rvalueRefSolvedAs;
	const Method method;

	Result(const string& ded, const string& as, Method meth)
		: deducedTemplateArg(ded), rvalueRefSolvedAs(as), method(meth)
	{}
};
bool operator==(const Result& lhs, const Result& rhs) {
	return lhs.method == rhs.method
	    && lhs.deducedTemplateArg == rhs.deducedTemplateArg
	    && lhs.rvalueRefSolvedAs == rhs.rvalueRefSolvedAs;
}


class C {
public:
	const string name;
	C(const char* name) : name(name) {}
};

Result::Method overloadedFunction(const C&) { return Result::Method::VAL; }
Result::Method overloadedFunction(C&)       { return Result::Method::REF; }


template <typename T> struct type;
template <>           struct type<C>       { static string name() { return "C"; } };
template <typename T> struct type<T&>      { static string name() { return type<T>::name() + "&"; } };
template <typename T> struct type<T&&>     { static string name() { return type<T>::name() + "&&"; } };
template <typename T> struct type<const T> { static string name() { return "const " + type<T>::name(); } };


template <typename T>
Result forwardingFunction(T&& arg) {
	string deducedTemplateArg = type<T>::name();
	string rvalueRefSolvedAs = type<T&&>::name();
	Result::Method method = overloadedFunction(forward<T>(arg));

	cout << "Calling forwardingFunction(T&& arg) with: " << arg.name << endl;
	cout << "\tDeduced T = " << deducedTemplateArg << endl;
	cout << "\tType of arg is T&& = " << deducedTemplateArg << " && = " << rvalueRefSolvedAs << endl;
	cout << "\tForwarded to: overloadedFunction(" << (method == Result::Method::VAL ? "const C&" : "C&") << ")" << endl;
	cout << endl;

	return {deducedTemplateArg, rvalueRefSolvedAs, method};
}

C         func()           { static C c{"C returned-by-function"};         return      c ; }
C&        funcRef()        { static C c{"C& returned-by-function"};        return      c ; }
C&&       funcRVRef()      { static C c{"C&& returned-by-function"};       return move(c); }
const C   funcConst()      { static C c{"const C returned-by-function"};   return      c ; }
const C&  funcConstRef()   { static C c{"const C& returned-by-function"};  return      c ; }
const C&& funcConstRVRef() { static C c{"const C&& returned-by-function"}; return move(c); }

int main() {
	C _var           = "C variable";         C         var           =      _var;
	C _varRef        = "C& variable";        C&        varRef        =      _varRef;
	C _varRVRef      = "C&& variable";       C&&       varRVRef      = move(_varRVRef);
	C _varConst      = "const C variable";   const C   varConst      =      _varConst;
	C _varConstRef   = "const C& variable";  const C&  varConstRef   =      _varConstRef;
	C _varConstRVRef = "const C&& variable"; const C&& varConstRVRef = move(_varConstRVRef);


	assert(forwardingFunction(C("C literal")) == Result("C", "C&&", Result::Method::VAL));

	assert(forwardingFunction(var)           == Result(      "C&",       "C&", Result::Method::REF));
	assert(forwardingFunction(varRef)        == Result(      "C&",       "C&", Result::Method::REF));
	assert(forwardingFunction(varRVRef)      == Result(      "C&",       "C&", Result::Method::REF));
	assert(forwardingFunction(varConst)      == Result("const C&", "const C&", Result::Method::VAL));
	assert(forwardingFunction(varConstRef)   == Result("const C&", "const C&", Result::Method::VAL));
	assert(forwardingFunction(varConstRVRef) == Result("const C&", "const C&", Result::Method::VAL));

	assert(forwardingFunction(func())           == Result(      "C" ,       "C&&", Result::Method::VAL));
	assert(forwardingFunction(funcRef())        == Result(      "C&",       "C&" , Result::Method::REF));
	assert(forwardingFunction(funcRVRef())      == Result(      "C" ,       "C&&", Result::Method::VAL));
	assert(forwardingFunction(funcConst())      == Result("const C" , "const C&&", Result::Method::VAL));
	assert(forwardingFunction(funcConstRef())   == Result("const C&", "const C&" , Result::Method::VAL));
	assert(forwardingFunction(funcConstRVRef()) == Result("const C" , "const C&&", Result::Method::VAL));

	cout << "OK" << endl;
}
