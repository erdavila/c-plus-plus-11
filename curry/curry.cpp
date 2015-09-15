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


int main() {
	oneArg::test();
	twoArgs::test();
}
