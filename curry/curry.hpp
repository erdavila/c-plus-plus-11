#ifndef __CURRY_HPP__
#define __CURRY_HPP__

#include <functional>

namespace curry {

	template <typename>
	class CurriedFunction;

	template <typename Return, typename Arg>
	class CurriedFunction<Return(Arg)> {
	private:
		using Function = Return(Arg);
		using Target = std::function<Function>;

		// Appliable to 1 argument of type (Arg)
		Target target;
	public:
		explicit CurriedFunction(Function function) : target(function) {}

		// Application to 1 argument
		Return operator()(const Arg& arg) const {
			return target(arg);
		}
	};

	// Appliable to 2..N arguments of types (Arg1, Arg2, RestArgs...)
	template <typename Return, typename Arg1, typename Arg2, typename... RestArgs>
	class CurriedFunction<Return(Arg1, Arg2, RestArgs...)>;


	template <typename Function>
	auto curry(Function* function) {
		return CurriedFunction<Function>(function);
	}
}

#endif
