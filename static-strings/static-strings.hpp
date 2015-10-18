#ifndef STATIC_STRINGS_HPP_
#define STATIC_STRINGS_HPP_

#include <string>
#include <type_traits>


namespace static_string {


namespace __impl {


	template <size_t Index, typename Char, Char... chars>
	struct char_at;

	template <size_t N, typename Char, Char x, Char... chars>
	struct find;

	template <size_t N, size_t Found, typename Char, Char x, Char... chars>
	struct rfind;

	template <size_t Pos, size_t Len, typename Char, Char... chars>
	struct substring;


} /* namespace __impl */


enum { NOT_FOUND = static_cast<size_t>(-1) };


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

	template <Char x>
	using find = typename __impl::find<0, Char, x>;

	template <Char x>
	using rfind = typename __impl::rfind<0, NOT_FOUND, Char, x>;

	template <size_t Pos, size_t Len>
	using substring = typename __impl::substring<Pos, Len, Char>::type;

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

	template <size_t Index>
	using char_at = typename __impl::char_at<Index, Char, c, chars...>;

	template <Char x>
	using find = typename __impl::find<0, Char, x, c, chars...>;

	template <Char x>
	using rfind = typename __impl::rfind<0, NOT_FOUND, Char, x, c, chars...>;

	template <size_t Pos, size_t Len>
	using substring = typename __impl::substring<Pos, Len, Char, c, chars...>::type;

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


	template <typename Char, Char c, Char... chars>
	struct char_at<0, Char, c, chars...> {
		static constexpr Char value = c;
	};

	template <size_t Index, typename Char, Char c, Char... chars>
	struct char_at<Index, Char, c, chars...>  {
		static constexpr Char value = char_at<Index - 1, Char, chars...>::value;
	};


	template <size_t N, typename Char, Char x>
	struct find<N, Char, x> {
		static constexpr size_t value = NOT_FOUND;
	};

	template <size_t N, typename Char, Char x, Char... chars>
	struct find<N, Char, x, x, chars...> {
		static constexpr size_t value = N;
	};

	template <size_t N, typename Char, Char x, Char c, Char... chars>
	struct find<N, Char, x, c, chars...> {
		static constexpr size_t value = find<N + 1, Char, x, chars...>::value;
	};


	template <size_t N, size_t Found, typename Char, Char x>
	struct rfind<N, Found, Char, x> {
		static constexpr size_t value = Found;
	};

	template <size_t N, size_t Found, typename Char, Char x, Char... chars>
	struct rfind<N, Found, Char, x, x, chars...> {
		static constexpr size_t value = rfind<N + 1, N, Char, x, chars...>::value;
	};

	template <size_t N, size_t Found, typename Char, Char x, Char c, Char... chars>
	struct rfind<N, Found, Char, x, c, chars...> {
		static constexpr size_t value = rfind<N + 1, Found, Char, x, chars...>::value;
	};


	template <typename Char, Char... chars>
	struct substring<0, 0, Char, chars...> {
		using type = static_string<Char>;
	};

	template <typename Char, Char c, Char... chars>
	struct substring<0, 0, Char, c, chars...> {
		using type = static_string<Char>;
	};

	template <size_t Len, typename Char, Char c, Char... chars>
	struct substring<0, Len, Char, c, chars...> {
	private:
		using first = static_string<Char, c>;
		using rest = typename substring<0, Len - 1, Char, chars...>::type;
	public:
		using type = typename concat<first, rest>::type;
	};

	template <size_t Pos, size_t Len, typename Char, Char c, Char... chars>
	struct substring<Pos, Len, Char, c, chars...> {
		using type = typename substring<Pos - 1, Len, Char, chars...>::type;
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
