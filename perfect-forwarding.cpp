#include <cassert>
#include <iostream>
#include <string>
#include <utility>
using namespace std;

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
	C(const char* name) : name(name) {}
};

string overloadedFunction(const C&) { return "const C&"; }
string overloadedFunction(C&)       { return "C&"; }


template <typename T> struct type;
template <>           struct type<C>       { static string name() { return "C"; } };
template <typename T> struct type<T&>      { static string name() { return type<T>::name() + "&"; } };
template <typename T> struct type<T&&>     { static string name() { return type<T>::name() + "&&"; } };
template <typename T> struct type<const T> { static string name() { return "const " + type<T>::name(); } };


template <typename T>
Result forwardingFunction(T&& arg) {
	string deducedTemplateArg = type<T>::name();
	string rvalueRefSolvedAs = type<T&&>::name();

	string overloadCalled = overloadedFunction(forward<T>(arg));

	cout << "Calling forwardingFunction(T&& arg) with: " << arg.name << endl;
	cout << "\tDeduced T = " << deducedTemplateArg << endl;
	cout << "\tType of arg is T&& = " << deducedTemplateArg << " && = " << rvalueRefSolvedAs << endl;
	cout << "\tForwarded to: overloadedFunction(" << overloadCalled << ")" << endl;
	cout << endl;
	return {deducedTemplateArg, rvalueRefSolvedAs, overloadCalled};
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


	assert(forwardingFunction(C("C literal")) == Result("C", "C&&", "const C&"));

	assert(forwardingFunction(var)           == Result(      "C&",       "C&",       "C&"));
	assert(forwardingFunction(varRef)        == Result(      "C&",       "C&",       "C&"));
	assert(forwardingFunction(varRVRef)      == Result(      "C&",       "C&",       "C&"));
	assert(forwardingFunction(varConst)      == Result("const C&", "const C&", "const C&"));
	assert(forwardingFunction(varConstRef)   == Result("const C&", "const C&", "const C&"));
	assert(forwardingFunction(varConstRVRef) == Result("const C&", "const C&", "const C&"));

	assert(forwardingFunction(func())           == Result(      "C" ,       "C&&", "const C&"));
	assert(forwardingFunction(funcRef())        == Result(      "C&",       "C&" ,       "C&"));
	assert(forwardingFunction(funcRVRef())      == Result(      "C" ,       "C&&", "const C&"));
	assert(forwardingFunction(funcConst())      == Result("const C" , "const C&&", "const C&"));
	assert(forwardingFunction(funcConstRef())   == Result("const C&", "const C&" , "const C&"));
	assert(forwardingFunction(funcConstRVRef()) == Result("const C" , "const C&&", "const C&"));

	cout << "OK" << endl;
}
