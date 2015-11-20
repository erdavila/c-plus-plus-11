#include <cassert>
#include <sstream>
#include <string>
#include "curry.hpp"


namespace oneArg {
	namespace argsByVal {
		std::string function(int i) {
			std::stringstream ss;
			ss << i;
			return ss.str();
		}

		void test() {
			auto curriedFunction = curry::curry(function);
			auto result = curriedFunction(7);
			assert(result == "7");
		}
	}

	namespace argsByRef {
		std::string str;

		std::string& function(int& i) {
			std::stringstream ss;
			ss << i;
			i++;
			str = ss.str();
			return str;
		}

		void test() {
			auto curriedFunction = curry::curry(function);

			int i = 7;
			auto& result = curriedFunction(i);

			assert(i == 8);
			assert(result == "7");
			assert(&result == &str);
		}
	}

	void test() {
		argsByVal::test();
		argsByRef::test();
	}
}


namespace twoArgs {
	namespace argsByVal {
		std::string function(int i, char c) {
			std::stringstream ss;
			ss << i << c;
			return ss.str();
		}

		void test_1_1() {
			auto curriedFunction = curry::curry(function);
			auto curriedFunction2 = curriedFunction(7);
			auto result = curriedFunction2('!');
			assert(result == "7!");
		}

		void test_2() {
			auto curriedFunction = curry::curry(function);
			auto result = curriedFunction(7, '!');
			assert(result == "7!");
		}

		void test() {
			test_1_1();
			test_2();
		}
	}

	namespace argsByRef {
		std::string str;

		std::string& function(int& i, char& c) {
			std::stringstream ss;
			ss << i << c;
			i++;
			c = '?';
			str = ss.str();
			return str;
		}

		void test_1_1() {
			auto curriedFunction = curry::curry(function);

			int i = -1;
			auto curriedFunction2 = curriedFunction(i);

			i = 7;
			char c = '!';
			auto& result = curriedFunction2(c);

			assert(i == 8);
			assert(c == '?');
			assert(result == "7!");
			assert(&result == &str);
		}

		void test_2() {
			auto curriedFunction = curry::curry(function);

			int i = 7;
			char c = '!';
			auto& result = curriedFunction(i, c);

			assert(i == 8);
			assert(c == '?');
			assert(result == "7!");
			assert(&result == &str);
		}

		void test() {
			test_1_1();
			test_2();
		}
	}

	void test() {
		argsByVal::test();
		argsByRef::test();
	}
}


namespace threeArgs {
	namespace argsByVal {
		std::string function(int i, char c, bool b) {
			std::stringstream ss;
			ss << (b ? '-' : '+') << i << c;
			return ss.str();
		}

		void test_1_1_1() {
			auto curriedFunction = curry::curry(function);
			auto curriedFunction2 = curriedFunction(7);
			auto curriedFunction3 = curriedFunction2('!');
			auto result = curriedFunction3(true);
			assert(result == "-7!");
		}

		void test_1_2() {
			auto curriedFunction = curry::curry(function);
			auto curriedFunction2 = curriedFunction(7);
			auto result = curriedFunction2('!', true);
			assert(result == "-7!");
		}

		void test_2_1() {
			auto curriedFunction = curry::curry(function);
			auto curriedFunction2 = curriedFunction(7, '!');
			auto result = curriedFunction2(true);
			assert(result == "-7!");
		}

		void test_3() {
			auto curriedFunction = curry::curry(function);
			auto result = curriedFunction(7, '!', true);
			assert(result == "-7!");
		}

		void test() {
			test_1_1_1();
			test_1_2();
			test_2_1();
			test_3();
		}
	}

	namespace argsByRef {
		std::string str;

		std::string& function(int& i, char& c, bool& b) {
			std::stringstream ss;
			ss << (b ? '-' : '+') << i << c;
			i++;
			c = '?';
			b = !b;
			str = ss.str();
			return str;
		}

		void test_1_1_1() {
			auto curriedFunction = curry::curry(function);

			int i = -1;
			auto curriedFunction2 = curriedFunction(i);

			char c = '@';
			auto curriedFunction3 = curriedFunction2(c);

			i = 7;
			c = '!';
			bool b = true;
			auto& result = curriedFunction3(b);

			assert(i == 8);
			assert(c == '?');
			assert(b == false);
			assert(result == "-7!");
			assert(&result == &str);
		}

		void test_1_2() {
			auto curriedFunction = curry::curry(function);

			int i = -1;
			auto curriedFunction2 = curriedFunction(i);

			i = 7;
			char c = '!';
			bool b = true;
			auto& result = curriedFunction2(c, b);

			assert(i == 8);
			assert(c == '?');
			assert(b == false);
			assert(result == "-7!");
			assert(&result == &str);
		}

		void test_2_1() {
			auto curriedFunction = curry::curry(function);

			int i = -1;
			char c = '@';
			auto curriedFunction2 = curriedFunction(i, c);

			i = 7;
			c = '!';
			bool b = true;
			auto& result = curriedFunction2(b);

			assert(i == 8);
			assert(c == '?');
			assert(b == false);
			assert(result == "-7!");
			assert(&result == &str);
		}

		void test_3() {
			auto curriedFunction = curry::curry(function);

			int i = 7;
			char c = '!';
			bool b = true;
			auto& result = curriedFunction(i, c, b);

			assert(i == 8);
			assert(c == '?');
			assert(b == false);
			assert(result == "-7!");
			assert(&result == &str);
		}

		void test() {
			test_1_1_1();
			test_1_2();
			test_2_1();
			test_3();
		}
	}

	void test() {
		argsByVal::test();
		argsByRef::test();
	}
}


namespace fourArgs {
	namespace argsByVal {
		std::string function(int i, char c, bool b, double d) {
			std::stringstream ss;
			ss << (b ? '-' : '+') << i << d << c;
			return ss.str();
		}

		void test_1_1_1_1() {
			auto curriedFunction = curry::curry(function);
			auto curriedFunction2 = curriedFunction(7);
			auto curriedFunction3 = curriedFunction2('!');
			auto curriedFunction4 = curriedFunction3(true);
			auto result = curriedFunction4(0.25);
			assert(result == "-70.25!");
		}

		void test_1_1_2() {
			auto curriedFunction = curry::curry(function);
			auto curriedFunction2 = curriedFunction(7);
			auto curriedFunction3 = curriedFunction2('!');
			auto result = curriedFunction3(true, 0.25);
			assert(result == "-70.25!");
		}

		void test_1_2_1() {
			auto curriedFunction = curry::curry(function);
			auto curriedFunction2 = curriedFunction(7);
			auto curriedFunction3 = curriedFunction2('!', true);
			auto result = curriedFunction3(0.25);
			assert(result == "-70.25!");
		}

		void test_1_3() {
			auto curriedFunction = curry::curry(function);
			auto curriedFunction2 = curriedFunction(7);
			auto result = curriedFunction2('!', true, 0.25);
			assert(result == "-70.25!");
		}

		void test_2_1_1() {
			auto curriedFunction = curry::curry(function);
			auto curriedFunction2 = curriedFunction(7, '!');
			auto curriedFunction3 = curriedFunction2(true);
			auto result = curriedFunction3(0.25);
			assert(result == "-70.25!");
		}

		void test_2_2() {
			auto curriedFunction = curry::curry(function);
			auto curriedFunction2 = curriedFunction(7, '!');
			auto result = curriedFunction2(true, 0.25);
			assert(result == "-70.25!");
		}

		void test_3_1() {
			auto curriedFunction = curry::curry(function);
			auto curriedFunction2 = curriedFunction(7, '!', true);
			auto result = curriedFunction2(0.25);
			assert(result == "-70.25!");
		}

		void test_4() {
			auto curriedFunction = curry::curry(function);
			auto result = curriedFunction(7, '!', true, 0.25);
			assert(result == "-70.25!");
		}

		void test() {
			test_1_1_1_1();
			test_1_1_2();
			test_1_2_1();
			test_1_3();
			test_2_1_1();
			test_2_2();
			test_3_1();
			test_4();
		}
	}

	namespace argsByRef {
		std::string str;

		std::string& function(int& i, char& c, bool& b, double& d) {
			std::stringstream ss;
			ss << (b ? '-' : '+') << i << d << c;
			i++;
			c = '?';
			b = !b;
			d = 2.5;
			str = ss.str();
			return str;
		}

		void test_1_1_1_1() {
			auto curriedFunction = curry::curry(function);

			int i = -1;
			auto curriedFunction2 = curriedFunction(i);

			char c = '@';
			auto curriedFunction3 = curriedFunction2(c);

			bool b = -1;
			auto curriedFunction4 = curriedFunction3(b);

			i = 7;
			c = '!';
			b = true;
			double d = 0.25;
			auto& result = curriedFunction4(d);

			assert(i == 8);
			assert(c == '?');
			assert(b == false);
			assert(d == 2.5);
			assert(result == "-70.25!");
			assert(&result == &str);
		}

		void test_1_1_2() {
			auto curriedFunction = curry::curry(function);

			int i = -1;
			auto curriedFunction2 = curriedFunction(i);

			char c = '@';
			auto curriedFunction3 = curriedFunction2(c);

			i = 7;
			c = '!';
			bool b = true;
			double d = 0.25;
			auto& result = curriedFunction3(b, d);

			assert(i == 8);
			assert(c == '?');
			assert(b == false);
			assert(d == 2.5);
			assert(result == "-70.25!");
			assert(&result == &str);
		}

		void test_1_2_1() {
			auto curriedFunction = curry::curry(function);

			int i = -1;
			auto curriedFunction2 = curriedFunction(i);

			char c = '@';
			bool b = -1;
			auto curriedFunction3 = curriedFunction2(c, b);

			i = 7;
			c = '!';
			b = true;
			double d = 0.25;
			auto& result = curriedFunction3(d);

			assert(i == 8);
			assert(c == '?');
			assert(b == false);
			assert(d == 2.5);
			assert(result == "-70.25!");
			assert(&result == &str);
		}

		void test_1_3() {
			auto curriedFunction = curry::curry(function);

			int i = -1;
			auto curriedFunction2 = curriedFunction(i);

			i = 7;
			char c = '!';
			bool b = true;
			double d = 0.25;
			auto& result = curriedFunction2(c, b, d);

			assert(i == 8);
			assert(c == '?');
			assert(b == false);
			assert(d == 2.5);
			assert(result == "-70.25!");
			assert(&result == &str);
		}

		void test_2_1_1() {
			auto curriedFunction = curry::curry(function);

			int i = -1;
			char c = '@';
			auto curriedFunction2 = curriedFunction(i, c);

			bool b = -1;
			auto curriedFunction3 = curriedFunction2(b);

			i = 7;
			c = '!';
			b = true;
			double d = 0.25;
			auto& result = curriedFunction3(d);

			assert(i == 8);
			assert(c == '?');
			assert(b == false);
			assert(d == 2.5);
			assert(result == "-70.25!");
			assert(&result == &str);
		}

		void test_2_2() {
			auto curriedFunction = curry::curry(function);

			int i = -1;
			char c = '@';
			auto curriedFunction2 = curriedFunction(i, c);

			i = 7;
			c = '!';
			bool b = true;
			double d = 0.25;
			auto& result = curriedFunction2(b, d);

			assert(i == 8);
			assert(c == '?');
			assert(b == false);
			assert(d == 2.5);
			assert(result == "-70.25!");
			assert(&result == &str);
		}

		void test_3_1() {
			auto curriedFunction = curry::curry(function);

			int i = -1;
			char c = '@';
			bool b = -1;
			auto curriedFunction2 = curriedFunction(i, c, b);

			i = 7;
			c = '!';
			b = true;
			double d = 0.25;
			auto& result = curriedFunction2(d);

			assert(i == 8);
			assert(c == '?');
			assert(b == false);
			assert(d == 2.5);
			assert(result == "-70.25!");
			assert(&result == &str);
		}

		void test_4() {
			auto curriedFunction = curry::curry(function);

			int i = 7;
			char c = '!';
			bool b = true;
			double d = 0.25;
			auto& result = curriedFunction(i, c, b, d);

			assert(i == 8);
			assert(c == '?');
			assert(b == false);
			assert(d == 2.5);
			assert(result == "-70.25!");
			assert(&result == &str);
		}

		void test() {
			test_1_1_1_1();
			test_1_1_2();
			test_1_2_1();
			test_1_3();
			test_2_1_1();
			test_2_2();
			test_3_1();
			test_4();
		}
	}

	void test() {
		argsByVal::test();
		argsByRef::test();
	}
}


namespace types {
	std::string function(int i, char c, bool b) {
		std::stringstream ss;
		ss << (b ? '-' : '+') << i << c;
		return ss.str();
	}

	namespace std_function {
		void test() {
			std::function<std::string(int, char, bool)> func(function);
			auto curriedFunction = curry::curry(func);
			auto curriedFunction2 = curriedFunction(7);
			auto curriedFunction3 = curriedFunction2('!');
			auto result = curriedFunction3(true);
			assert(result == "-7!");
		}
	}

	namespace memberFunction {
#if __clang__
	/*
	 * As of writing this, the installed version of libc++ is "1.0~svn199600-1".
	 * It has some issue that prevents std::function to correctly work with
	 * cv-qualified member functions.
	 * See http://stackoverflow.com/q/33838807/747919
	 */
# define IGNORE_CV_QUALIFIED_MEMBERS
#endif

		struct Class {
			auto member(int i, char c, bool b) { return function(i, c, b); }
			auto memberConst(int i, char c, bool b) const { return function(i, c, b); }
			auto memberVolatile(int i, char c, bool b) volatile { return function(i, c, b); }
			auto memberConstVolatile(int i, char c, bool b) const volatile { return function(i, c, b); }
		};

		Class instance;

		void testMember() {
			auto curriedFunction = curry::curry(&Class::member);
			auto curriedFunction2 = curriedFunction(instance);
			auto curriedFunction3 = curriedFunction2(7);
			auto curriedFunction4 = curriedFunction3('!');
			auto result = curriedFunction4(true);
			assert(result == "-7!");
		}

#ifndef IGNORE_CV_QUALIFIED_MEMBERS
		void testMemberConst() {
			auto curriedFunction = curry::curry(&Class::memberConst);
			auto curriedFunction2 = curriedFunction(instance);
			auto curriedFunction3 = curriedFunction2(7);
			auto curriedFunction4 = curriedFunction3('!');
			auto result = curriedFunction4(true);
			assert(result == "-7!");
		}

		void testMemberVolatile() {
			auto curriedFunction = curry::curry(&Class::memberVolatile);
			auto curriedFunction2 = curriedFunction(instance);
			auto curriedFunction3 = curriedFunction2(7);
			auto curriedFunction4 = curriedFunction3('!');
			auto result = curriedFunction4(true);
			assert(result == "-7!");
		}

		void testMemberConstVolatile() {
			auto curriedFunction = curry::curry(&Class::memberConstVolatile);
			auto curriedFunction2 = curriedFunction(instance);
			auto curriedFunction3 = curriedFunction2(7);
			auto curriedFunction4 = curriedFunction3('!');
			auto result = curriedFunction4(true);
			assert(result == "-7!");
		}
#endif

		void test() {
			testMember();
#ifndef IGNORE_CV_QUALIFIED_MEMBERS
			testMemberConst();
			testMemberVolatile();
			testMemberConstVolatile();
#endif
		}
	}

	namespace lambda {
		void test() {
			auto lambda = [](int i, char c, bool b) { return function(i, c, b); };
			auto curriedFunction = curry::curry<std::string(int, char, bool)>(lambda);
			auto curriedFunction2 = curriedFunction(7);
			auto curriedFunction3 = curriedFunction2('!');
			auto result = curriedFunction3(true);
			assert(result == "-7!");
		}
	}

	namespace functionObject {
		struct Class {
			auto operator()(int i, char c, bool b) { return function(i, c, b); }
		};

		void test() {
			Class instance;
			auto curriedFunction = curry::curry<std::string(int, char, bool)>(instance);
			auto curriedFunction2 = curriedFunction(7);
			auto curriedFunction3 = curriedFunction2('!');
			auto result = curriedFunction3(true);
			assert(result == "-7!");
		}
	}

	namespace constructor {
		struct Class {
			std::string result;
			Class(int i, char c, bool b)
				: result(std::move(function(i, c, b)))
			{}

			Class(Class&& that)
				: result(std::move(that.result))
			{}

			Class(const Class&) = delete;
		};

		void test() {
			auto curriedFunction = curry::curry<Class(int, char, bool)>();
			auto curriedFunction2 = curriedFunction(7);
			auto curriedFunction3 = curriedFunction2('!');
			auto instance = std::move(curriedFunction3(true));
			assert(instance.result == "-7!");
		}
	}

	void test() {
		std_function::test();
		memberFunction::test();
		lambda::test();
		functionObject::test();
		constructor::test();
	}
}


int main() {
	oneArg::test();
	twoArgs::test();
	threeArgs::test();
	fourArgs::test();
	types::test();
}
