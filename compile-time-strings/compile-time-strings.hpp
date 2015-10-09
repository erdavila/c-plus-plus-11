#ifndef COMPILE_TIME_STRINGS_HPP_
#define COMPILE_TIME_STRINGS_HPP_

#include <string>
#include <type_traits>


namespace ctstring {


template <typename Char, Char...>
struct ctstring_impl;

template <typename Char>
struct ctstring_impl<Char> {
	using char_type = Char;
	using string_type = std::basic_string<Char>;

	enum { size = 0 };

	inline static string_type string() {
		return {};
	}

private:

	template <typename C, C...> friend struct ctstring_impl;

	inline static void append_to(string_type& string) {}
};

template <typename Char, Char c, Char... chars>
struct ctstring_impl<Char, c, chars...> {
	using char_type = Char;
	using string_type = std::basic_string<Char>;

	enum { size = 1 + sizeof...(chars) };

	inline static string_type string() {
		string_type result;
		result.reserve(size);
		append_to(result);
		return result;
	}

private:

	template <typename C, C...> friend struct ctstring_impl;

	inline static void append_to(string_type& string) {
		string += c;
		ctstring_impl<Char, chars...>::append_to(string);
	}
};


namespace __impl {


	template <typename StrProvider, size_t len, typename Char, char... chars>
	struct make_ctstring {
		using type = typename make_ctstring<StrProvider,
		                                   len - 1,
		                                   Char,
		                                   StrProvider::str()[len - 1],
		                                   chars...
		                                  >::type;
	};

	template <typename StrProvider, typename Char, char... chars>
	struct make_ctstring<StrProvider, 0, Char, chars...> {
		using type = ctstring_impl<Char, chars...>;
	};


	template <typename Char>
	constexpr size_t str_length(const Char* str) {
		return str[0] == Char(0) ? 0 : (1 + str_length(str + 1));
	}


	template <typename... CTStrings>
	struct concat;

	template <typename Char, Char... chars>
	struct concat<ctstring_impl<Char, chars...>> {
		using type = ctstring_impl<Char, chars...>;
	};

	template <typename Char, Char... chars1, Char... chars2, typename... CTStrings>
	struct concat<ctstring_impl<Char, chars1...>, ctstring_impl<Char, chars2...>, CTStrings...> {
		using type = typename concat<ctstring_impl<Char, chars1..., chars2...>, CTStrings...>::type;
	};


} /* namespace __impl */


template <typename StrProvider>
using ctstring = typename __impl::make_ctstring<StrProvider,
                                                __impl::str_length(StrProvider::str()),
                                                typename std::decay<decltype(StrProvider::str()[0])>::type
                                               >::type;

template <typename... CTStrings>
using concat = typename __impl::concat<CTStrings...>::type;


} /* namespace ctstring */


#endif /* COMPILE_TIME_STRINGS_HPP_ */
