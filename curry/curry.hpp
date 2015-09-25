#ifndef __CURRY_HPP__
#define __CURRY_HPP__

#include <functional>
#include <type_traits>

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

	template <typename Target, typename Return, typename BoundArg, typename NextArg, typename AfterNextArg, typename... RestArgs>
	class BoundFunction<Target, Return(BoundArg, NextArg, AfterNextArg, RestArgs...)> {
	private:
		using This = BoundFunction<Target, Return(BoundArg, NextArg, AfterNextArg, RestArgs...)>;

		// Appliable to 3..N arguments of types (BoundArg, NextArg, AfterNextArg, RestArgs...)
		Target target;

		BoundArg boundArg;
	public:
		BoundFunction(const Target& target, BoundArg boundArg)
			: target(target), boundArg(boundArg)
			{}

		// Application to 1 argument
		auto operator()(NextArg nextArg) const {
			return BoundFunction<This, Return(NextArg, AfterNextArg, RestArgs...)>(*this, nextArg);
		}

		// Application to 2..N-2 arguments
		template <typename... OtherArgs>
		auto operator()(NextArg nextArg, AfterNextArg afterNextArg, OtherArgs&&... otherArgs) const
			-> typename std::enable_if<sizeof...(OtherArgs) < sizeof...(RestArgs),
			     decltype(this->operator()(nextArg)(afterNextArg, std::forward<OtherArgs>(otherArgs)...))
			   >::type
		{
			return        this->operator()(nextArg)(afterNextArg, std::forward<OtherArgs>(otherArgs)...);
		}

		// Application to N-1 arguments
		Return operator()(NextArg nextArg, AfterNextArg afterNextArg, RestArgs... restArgs) const {
			return target(boundArg, nextArg, afterNextArg, restArgs...);
		}
	};


	template <typename>
	class CurriedFunction;

	template <typename Return, typename Arg>
	class CurriedFunction<Return(Arg)> {
	private:
		using Signature = Return(Arg);
		using Function = std::function<Signature>;

		// Appliable to 1 argument of type (Arg)
		Function target;
	public:
		explicit CurriedFunction(const Function& function) : target(function) {}
		explicit CurriedFunction(Function& function) : target(std::move(function)) {}

		// Application to 1 argument
		Return operator()(const Arg& arg) const {
			return target(arg);
		}
	};

	template <typename Return, typename Arg1, typename Arg2, typename... RestArgs>
	class CurriedFunction<Return(Arg1, Arg2, RestArgs...)> {
	private:
		using Signature = Return(Arg1, Arg2, RestArgs...);
		using Function = std::function<Signature>;
		using This = CurriedFunction<Signature>;

		// Appliable to 2..N arguments of types (Arg1, Arg2, RestArgs...)
		Function target;
	public:
		explicit CurriedFunction(const Function& function) : target(function) {}
		explicit CurriedFunction(Function& function) : target(std::move(function)) {}

		// Application to 1 argument
		auto operator()(Arg1 arg1) const {
			return BoundFunction<This, Signature>(*this, arg1);
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


	template <typename Return, typename... Args>
	auto curry(std::function<Return(Args...)>&& function) {
		return CurriedFunction<Return(Args...)>(std::move(function));
	}

	template <typename Return, typename... Args>
	auto curry(const std::function<Return(Args...)>& function) {
		return CurriedFunction<Return(Args...)>(function);
	}

	template <typename Return, typename... Args>
	auto curry(Return(*func)(Args...)) {
		std::function<Return(Args...)> function = func;
		return curry(std::move(function));
	}

	template <typename Return, typename Class, typename... Args>
	auto curry(Return(Class::*member)(Args...)) {
		std::function<Return(Class&, Args...)> function = member;
		return curry(std::move(function));
	}

	template <typename Return, typename Class, typename... Args>
	auto curry(Return(Class::*member)(Args...) const) {
		std::function<Return(Class&, Args...)> function = member;
		return curry(std::move(function));
	}

	template <typename Return, typename Class, typename... Args>
	auto curry(Return(Class::*member)(Args...) volatile) {
		std::function<Return(Class&, Args...)> function = member;
		return curry(std::move(function));
	}

	template <typename Return, typename Class, typename... Args>
	auto curry(Return(Class::*member)(Args...) const volatile) {
		std::function<Return(Class&, Args...)> function = member;
		return curry(std::move(function));
	}

	template <typename Signature, typename Type>
	auto curry(Type value) {
		std::function<Signature> function = value;
		return curry(std::move(function));
	}


	template <typename>
	struct ConstructorProxy;

	template <typename Return, typename... Args>
	struct ConstructorProxy<Return(Args...)> {
		Return operator()(Args&&... args) const {
			return std::move(Return(std::forward<Args>(args)...));
		}
	};

	template <typename Signature>
	auto curry() {
		ConstructorProxy<Signature> proxy;
		std::function<Signature> function = proxy;
		return curry(std::move(function));
	}
}

#endif
