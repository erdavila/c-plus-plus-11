#ifndef __CURRY_HPP__
#define __CURRY_HPP__

#include <functional>

namespace curry {

	template <typename, typename>
	class BoundFunction;

	template <typename Target, typename Return, typename BoundArg, typename LastArg>
	class BoundFunction<Target, Return(BoundArg, LastArg)> {
	private:
		// Appliable to 2 arguments of types (BoundArg, LastArg)
		Target target;

		BoundArg boundArg;
	public:
		BoundFunction(const Target& target, BoundArg boundArg)
			: target(target), boundArg(boundArg)
			{}

		// Application to 1 argument
		Return operator()(LastArg lastArg) const {
			return target(boundArg, lastArg);
		}
	};

	// Appliable to 2..N arguments of types (NextArg, AfterNextArg, RestArgs...)
	template <typename Target, typename Return, typename BoundArg, typename NextArg, typename AfterNextArg, typename... RestArgs>
	class BoundFunction<Target, Return(BoundArg, NextArg, AfterNextArg, RestArgs...)>;


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

	template <typename Return, typename Arg1, typename Arg2, typename... RestArgs>
	class CurriedFunction<Return(Arg1, Arg2, RestArgs...)> {
	private:
		using Function = Return(Arg1, Arg2, RestArgs...);
		using Target = std::function<Function>;
		using This = CurriedFunction<Function>;

		// Appliable to 2..N arguments of types (Arg1, Arg2, RestArgs...)
		Target target;
	public:
		explicit CurriedFunction(Function function) : target(function) {}

		// Application to 1 argument
		auto operator()(Arg1 arg1) const {
			return BoundFunction<This, Function>(*this, arg1);
		}

		// Application to 2..N-1 arguments
		template <typename... OtherArgs>
		auto operator()(Arg1 arg1, Arg2 arg2, OtherArgs&&... otherArgs)
			-> typename std::enable_if<sizeof...(OtherArgs) < sizeof...(RestArgs),
			     decltype(this->operator()(arg1)(arg2, std::forward<OtherArgs>(otherArgs)...))
			   >::type
		{
			return        this->operator()(arg1)(arg2, std::forward<OtherArgs>(otherArgs)...);
		}

		// Application to N arguments
		Return operator()(Arg1 arg1, Arg2 arg2, RestArgs... restArgs) const {
			return target(arg1, arg2, restArgs...);
		}
	};


	template <typename Function>
	auto curry(Function* function) {
		return CurriedFunction<Function>(function);
	}
}

#endif
