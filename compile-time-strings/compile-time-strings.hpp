#ifndef COMPILE_TIME_STRINGS_HPP_
#define COMPILE_TIME_STRINGS_HPP_

#include <string>


namespace ctstring {


template <char...>
struct ctstring_impl;

template <>
struct ctstring_impl<> {
	enum { size = 0 };

	inline static std::string string() {
		return "";
	}
};

template <char c, char... chars>
struct ctstring_impl<c, chars...> {
	enum { size = 1 + sizeof...(chars) };

	inline static std::string string() {
		return c + ctstring_impl<chars...>::string();
	}
};


namespace __impl {


	template <typename StrProvider, size_t len, char... chars>
	struct make_ctstring {
		using type = typename make_ctstring<StrProvider,
										   len - 1,
										   StrProvider::str()[len - 1],
										   chars...
										  >::type;
	};

	template <typename StrProvider, char... chars>
	struct make_ctstring<StrProvider, 0, chars...> {
		using type = ctstring_impl<chars...>;
	};


	constexpr size_t str_length(const char* str) {
		return str[0] == '\0' ? 0 : (1 + str_length(str + 1));
	}


	template <typename... CTStrings>
	struct concat;

	template <>
	struct concat<> {
		using type = ctstring_impl<>;
	};

	template <char... chars>
	struct concat<ctstring_impl<chars...>> {
		using type = ctstring_impl<chars...>;
	};

	template <char... chars1, char... chars2, typename... CTStrings>
	struct concat<ctstring_impl<chars1...>, ctstring_impl<chars2...>, CTStrings...> {
		using type = typename concat<ctstring_impl<chars1..., chars2...>, CTStrings...>::type;
	};


} /* namespace __impl */


template <typename StrProvider>
using ctstring = typename __impl::make_ctstring<StrProvider,
                                                __impl::str_length(StrProvider::str())
                                               >::type;

template <typename... CTStrings>
using concat = typename __impl::concat<CTStrings...>::type;


} /* namespace ctstring */


#endif /* COMPILE_TIME_STRINGS_HPP_ */
