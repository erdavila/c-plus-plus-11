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


int main() {
	oneArg::test();
}
