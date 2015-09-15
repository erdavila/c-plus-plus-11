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


int main() {
	oneArg::test();
	twoArgs::test();
	threeArgs::test();
	fourArgs::test();
}
