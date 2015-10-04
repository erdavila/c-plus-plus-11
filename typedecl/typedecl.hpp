#ifndef __ARRAYS_HPP__
#define __ARRAYS_HPP__

#include <string>
#include <type_traits>

#ifdef __CYGWIN__
#include <sstream>
namespace std {
	inline std::string to_string(unsigned long val) { ostringstream os; os << val; return os.str(); }
}
#endif


namespace {
namespace __typedecl {


enum type_type { BASICTYPE, COMPOSITION };
struct basic_type { static constexpr type_type type = BASICTYPE; };
struct composition { static constexpr type_type type = COMPOSITION; };


template <typename T>
struct impl;


template <typename T, type_type = impl<T>::type>
struct prefix_cv_qual_if_basictype;

template <typename T>
struct prefix_cv_qual_if_basictype<T, BASICTYPE> {
	inline static std::string value(const std::string& cv_qual, const std::string& suffix) {
		return impl<T>::value(cv_qual, suffix);
	}
};

template <typename T>
struct prefix_cv_qual_if_basictype<T, COMPOSITION> {
	inline static std::string value(const std::string& cv_qual, const std::string& suffix) {
		return impl<T>::value(cv_qual + suffix);
	}
};


template <typename T>
struct impl<const T> {
	inline static std::string value(const std::string& suffix = "") {
		return prefix_cv_qual_if_basictype<T>::value("const", suffix);
	}
};

template <typename T>
struct impl<volatile T> {
	inline static std::string value(const std::string& suffix = "") {
		return prefix_cv_qual_if_basictype<T>::value("volatile", suffix);
	}
};

// Required to disambiguate between <const T> and <volatile T>
template <typename T>
struct impl<const volatile T> {
	inline static std::string value(const std::string& suffix = "") {
		return prefix_cv_qual_if_basictype<T>::value("const volatile", suffix);
	}
};


template <typename T, bool = std::is_array<T>::value>
struct parenthesize_if_array;

template <typename T>
struct parenthesize_if_array<T, false> {
	inline static std::string value(const std::string& arg) {
		return impl<T>::value(arg);
	}
};

template <typename T>
struct parenthesize_if_array<T, true> {
	inline static std::string value(const std::string& arg) {
		return impl<T>::value("(" + arg + ")");
	}
};


template <typename T>
struct impl<T*> : composition {
	inline static std::string value(const std::string& suffix = "") {
		return parenthesize_if_array<T>::value("*" + suffix);
	}
};

template <typename T>
struct impl<T&> {
	inline static std::string value() {
		return parenthesize_if_array<T>::value("&");
	}
};

template <typename T>
struct impl<T&&> {
	inline static std::string value() {
		return parenthesize_if_array<T>::value("&&");
	}
};


template <typename T>
struct array_impl : composition {
	inline static std::string value(const std::string& prefix = "") {
		return impl<T>::value(prefix + "[]");
	}
};

template <typename T>
struct impl<T[]> : array_impl<T> {};

// Required to disambiguate between <const T> and <T[]>
template <typename T>
struct impl<const T[]> : array_impl<const T> {};

// Required to disambiguate between <volatile T> and <T[]>
template <typename T>
struct impl<volatile T[]> : array_impl<volatile T> {};

// Required to disambiguate between <const T>, <volatile T>, <const volatile T>,  <T[]>, <const T[]> and <volatile T[]>
template <typename T>
struct impl<const volatile T[]> : array_impl<const volatile T> {};


template <typename T, size_t N>
struct sized_array_impl : composition {
	inline static std::string value(const std::string& prefix = "") {
		return impl<T>::value(prefix + "[" + std::to_string(N) + "]");
	}
};

template <typename T, size_t N>
struct impl<T[N]> : sized_array_impl<T, N> {};

// Required to disambiguate between <const T> and <T[N]>
template <typename T, size_t N>
struct impl<const T[N]> : sized_array_impl<const T, N> {};

// Required to disambiguate between <volatile T> and <T[N]>
template <typename T, size_t N>
struct impl<volatile T[N]> : sized_array_impl<volatile T, N> {};

// Required to disambiguate between <const T>, <volatile T>, <const volatile T>,  <T[N]>, <const T[N]> and <volatile T[N]>
template <typename T, size_t N>
struct impl<const volatile T[N]> : sized_array_impl<const volatile T, N> {};


} /* namespace __typedecl */
} /* unnamed namespace */


template <typename T>
inline std::string typedecl() {
	return __typedecl::impl<T>::value();
}


#define DEFINE_TYPEDECL(T) \
namespace { \
namespace __typedecl { \
template <> \
struct impl<T> : basic_type { \
	inline static std::string value(const std::string& suffix = "") { \
		return #T + suffix; \
	} \
	inline static std::string value(const std::string& cv_qual, const std::string& suffix) { \
		return cv_qual + " " #T + suffix; \
	} \
}; \
} /* namespace __typedecl */ \
} /* unnamed namespace */


DEFINE_TYPEDECL(int);


#endif