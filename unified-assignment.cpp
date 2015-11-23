#include <iostream>

using namespace std;


class TracerBase {
protected:
	static int enabled;

	string name;

	TracerBase(const string& name) : name(name) {}

	~TracerBase() {
		--enabled;
		doLog("END " + name);
		if(enabled == 0) {
			doLog(repeat("=", 80));
		}
	}

	static void doLog(const string& str = "") {
		string indent = repeat("  ", enabled);
		cout << indent << str << endl;
	}

	static string repeat(const string& s, int n) {
		string str;
		for(int i = 0; i < n; ++i) {
			str += s;
		}
		return str;
	}
};
int TracerBase::enabled = 0;


template <typename C>
class Tracer : TracerBase {
public:
	Tracer(const string& function, const string& args = "")
		: TracerBase(method(function, args))
	{
		doLog("BEGIN " + name);
		++enabled;
	}

	static void trace(const string& function, const string& args = "") {
		if(enabled > 0) {
			doLog(method(function, args));
		}
	}

private:
	static string method(const string& function, const string& args) {
		string m = C::NAME + "::" + function + "(" + args + ")";
		if(function == C::NAME) {
			m += "  // Object is constructed!";
		}
		return m;
	}
};


template <typename T = void>
class Base {
public:
	static void assignFromLvalue() {
		T v1, v2;
		Tracer t(__FUNCTION__);
		v1 = v2;
	}

	static void assignFromRvalue() {
		T v1, v2;
		Tracer t(__FUNCTION__);
		v1 = id(v2);
	}

protected:
	typedef ::Tracer<T> Tracer;

	Base()            { Tracer::trace(T::NAME); }
	Base(const Base&) { Tracer::trace(T::NAME, "const " + T::NAME + "&"); }
#if __cplusplus >= 201103L
	Base(Base&&)      { Tracer::trace(T::NAME, T::NAME + "&&"); }
#endif

	~Base() { Tracer::trace("~" + T::NAME); }

	void swap(Base& s) {
		Tracer::trace(__FUNCTION__, T::NAME + "&");
	}

private:
	static T id(T& v) {
		return true ? v : v;
	}
};


//#define SEPARATED_HAS_MOVE_ASSIGNMENT

class Separated : public Base<Separated> {
public:
	static const string NAME;

	Separated()                   : Base<Separated>() {}
	Separated(const Separated& s) : Base<Separated>(s) {}
#if __cplusplus >= 201103L
	Separated(Separated&& s)      : Base<Separated>(std::move(s)) {}
#endif

	Separated& operator=(const Separated& s) {
		Tracer t(__FUNCTION__, "const Separated&");
		if(!sameAddress(s)) {
			Separated(s).swap(*this);
		}
		return *this;
	}

#if __cplusplus >= 201103L && defined(SEPARATED_HAS_MOVE_ASSIGNMENT)
	Separated& operator=(Separated&& s) & {
		Tracer t(__FUNCTION__, "Separated&&");
		s.swap(*this);
		return *this;
	}
#endif

private:
	bool sameAddress(const Separated& s) const {
		Tracer::trace(__FUNCTION__, "const Separated&");
		return false;
	}
};
const string Separated::NAME = "Separated";


class Unified : public Base<Unified> {
public:
	static const string NAME;

	Unified()                 : Base<Unified>() {}
	Unified(const Unified& u) : Base<Unified>(u) {}
#if __cplusplus >= 201103L
	Unified(Unified&& u)      : Base<Unified>(std::move(u)) {}
#endif

	Unified& operator=(Unified u) {
		Tracer t(__FUNCTION__, "Unified u");
		u.swap(*this);
		return *this;
	}
};
const string Unified::NAME = "Unified";


int main() {
	Separated::assignFromLvalue();
	Unified::assignFromLvalue();
	Separated::assignFromRvalue();
	Unified::assignFromRvalue();
}
