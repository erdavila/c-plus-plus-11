#ifndef __CURRY_HPP__
#define __CURRY_HPP__

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace curry {

	template <size_t N, typename Tuple, typename... Types>
	struct tuple_from_types_impl;

	template <size_t N, typename... TupleTypes, typename Type, typename... Types>
	struct tuple_from_types_impl<N, std::tuple<TupleTypes...>, Type, Types...>
		: tuple_from_types_impl<N - 1, std::tuple<TupleTypes..., Type>, Types...>
	{};

	template <typename... TupleTypes, typename Type, typename... Types>
	struct tuple_from_types_impl<0, std::tuple<TupleTypes...>, Type, Types...> {
		using type = std::tuple<TupleTypes...>;
	};

	template <typename... TupleTypes, typename... Types>
	struct tuple_from_types_impl<0, std::tuple<TupleTypes...>, Types...> {
		using type = std::tuple<TupleTypes...>;
	};

	template <size_t N, typename... Types>
	using tuple_from_types = typename tuple_from_types_impl<N, std::tuple<>, Types...>::type;


	template <bool AllArgsProvided>
	class Applicator;


	template <typename Signature, size_t BoundArgsCount = 0>
	class CurriedFunction;

	template <typename Return, typename... Args, size_t BoundArgsCount>
	class CurriedFunction<Return(Args...), BoundArgsCount> {
	private:
		static_assert(BoundArgsCount < sizeof...(Args), "Too many bound arguments");

		using Signature = Return(Args...);
		using Function = std::function<Signature>;
		using BoundArgsTuple = tuple_from_types<BoundArgsCount, Args...>;

		Function function;
		BoundArgsTuple boundArgs;

		template <typename... GivenArgs>
		struct bound_args_with {
			static constexpr size_t count = BoundArgsCount + sizeof...(GivenArgs);
			static constexpr bool satisfies = (count >= sizeof...(Args));
		};

		template <bool> friend class Applicator;

	public:
		explicit CurriedFunction(const Function& function, const BoundArgsTuple& boundArgs = std::tuple<>())
			: function(function), boundArgs(boundArgs)
		{}

		template <typename... GivenArgs>
		typename std::conditional<bound_args_with<GivenArgs...>::satisfies,
					Return,
					CurriedFunction<Signature, bound_args_with<GivenArgs...>::count>
			>::type
		operator()(GivenArgs&&... givenArgs) {
			static_assert(bound_args_with<GivenArgs...>::count <= sizeof...(Args), "Too many arguments");

			constexpr bool allArgsProvided = bound_args_with<GivenArgs...>::satisfies;
			return Applicator<allArgsProvided>::apply(*this, std::forward<GivenArgs>(givenArgs)...);
		}
	};


	template <size_t...> class index_sequence {};

	template <size_t N, typename Indexes>
	struct make_index_sequence_impl;

	template <size_t... Indexes>
	struct make_index_sequence_impl<0, index_sequence<Indexes...>> {
		using type = index_sequence<Indexes...>;
	};

	template <size_t N, size_t... Indexes>
	struct make_index_sequence_impl<N, index_sequence<Indexes...>>
		: make_index_sequence_impl<N - 1, index_sequence<N - 1, Indexes...>>
	{};

	template <size_t N>
	using make_index_sequence = typename make_index_sequence_impl<N, index_sequence<>>::type;


	template <>
	class Applicator<true> {
	private:
		template <typename Function, size_t... Indexes, typename BoundArgsTuple, typename... GivenArgs>
		static
		typename Function::result_type
		apply(const Function& function, index_sequence<Indexes...>, const BoundArgsTuple& boundArgs, GivenArgs&&... givenArgs) {
			return function(std::get<Indexes>(boundArgs)..., std::forward<GivenArgs>(givenArgs)...);
		}

	public:
		template <typename Return, typename... Args, size_t BoundArgsCount, typename... GivenArgs>
		static Return apply(const CurriedFunction<Return(Args...), BoundArgsCount>& cf, GivenArgs&&... givenArgs) {
			return apply(cf.function, make_index_sequence<BoundArgsCount>(), cf.boundArgs, std::forward<GivenArgs>(givenArgs)...);
		}
	};

	template <>
	class Applicator<false> {
	public:
		template <typename Signature, size_t BoundArgsCount, typename... GivenArgs>
		static auto apply(const CurriedFunction<Signature, BoundArgsCount>& cf, GivenArgs&&... givenArgs) {
			enum { NewBoundArgsCount = BoundArgsCount + sizeof...(GivenArgs) };
			using GivenArgsTuple = std::tuple<GivenArgs...>;
			return CurriedFunction<Signature, NewBoundArgsCount>(
					cf.function,
					std::tuple_cat(cf.boundArgs, GivenArgsTuple(std::forward<GivenArgs>(givenArgs)...))
			);
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
