#ifndef STATIC_STRINGS_HPP_
#define STATIC_STRINGS_HPP_

#include <string>
#include <type_traits>


namespace static_string {


template <typename Char, Char...>
struct static_string;

template <typename Char>
struct static_string<Char> {
	using char_type = Char;
	using string_type = std::basic_string<Char>;

	enum { size = 0 };

	inline static string_type string() {
		return {};
	}

private:

	template <typename C, C...> friend struct static_string;

	inline static void append_to(string_type& string) {}
};

template <typename Char, Char c, Char... chars>
struct static_string<Char, c, chars...> {
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

	template <typename C, C...> friend struct static_string;

	inline static void append_to(string_type& string) {
		string += c;
		static_string<Char, chars...>::append_to(string);
	}
};


namespace __impl {


	template <typename StrProvider, size_t len, typename Char, char... chars>
	struct build_from_provider {
		using type = typename build_from_provider<StrProvider,
		                                          len - 1,
		                                          Char,
		                                          StrProvider::str()[len - 1],
		                                          chars...
		                                         >::type;
	};

	template <typename StrProvider, typename Char, char... chars>
	struct build_from_provider<StrProvider, 0, Char, chars...> {
		using type = static_string<Char, chars...>;
	};


	template <typename StrProvider>
	struct has_size {
		template <size_t N> struct Check;
		template <typename T> static constexpr bool test(Check<T::size>*) { return true; }  // SFINAE!
		template <typename T> static constexpr bool test(...) { return false; }

		static constexpr bool value = test<StrProvider>(nullptr);
	};

	template <typename StrProvider, bool = has_size<StrProvider>::value>
	struct size_for;

	template <typename StrProvider>
	struct size_for<StrProvider, true> {
		enum { value = StrProvider::size };
	};

	template <typename StrProvider>
	struct size_for<StrProvider, false> {
		template <typename Char>
		static constexpr size_t str_length(const Char* str) {
			return str[0] == Char(0) ? 0 : (1 + str_length(str + 1));
		}

		enum { value = str_length(StrProvider::str()) };
	};


	template <typename... SSs>
	struct concat;

	template <typename Char, Char... chars>
	struct concat<static_string<Char, chars...>> {
		using type = static_string<Char, chars...>;
	};

	template <typename Char, Char... chars1, Char... chars2, typename... SSs>
	struct concat<static_string<Char, chars1...>, static_string<Char, chars2...>, SSs...> {
		using type = typename concat<static_string<Char, chars1..., chars2...>, SSs...>::type;
	};


} /* namespace __impl */


template <typename StrProvider, size_t len = __impl::size_for<StrProvider>::value>
using from_provider = typename __impl::build_from_provider<StrProvider,
                                                           len,
                                                           typename std::decay<decltype(StrProvider::str()[0])>::type
                                                          >::type;

template <typename... SSs>
using concat = typename __impl::concat<SSs...>::type;


} /* namespace static_string */


#endif /* STATIC_STRINGS_HPP_ */
