#include <cassert>
#include <type_traits>

/*
 * Implemented following the article "C++ auto and decltype Explained" by Thomas Becker.
 * http://thbecker.net/articles/auto_and_decltype/section_01.html
 */


enum value_category {
	LVALUE, XVALUE, PRVALUE
};

#define value_category_of(expr) \
	(std::is_lvalue_reference<decltype((expr))>::value \
		? LVALUE \
		: (std::is_rvalue_reference<decltype((expr))>::value \
			? XVALUE \
			: PRVALUE \
		  ) \
	)


template <typename T> T returns() {
	using U = typename std::decay<T>::type;
	U i;
	U* p = &i;
	return static_cast<T>(*p);
}


struct S {
	volatile int i;
	void f(char) {}
};


template <typename T> auto       tmpl(T arg)              -> decltype(arg);
template <typename T> auto       tmpl_lref(T& arg)        -> decltype(arg);
template <typename T> auto       tmpl_rref(T&& arg)       -> decltype(arg);
template <typename T> auto const_tmpl(const T arg)        -> decltype(arg);
template <typename T> auto const_tmpl_lref(const T& arg)  -> decltype(arg);
template <typename T> auto const_tmpl_rref(const T&& arg) -> decltype(arg);


#if defined(__cpp_decltype_auto) && __cpp_decltype_auto >= 201304
# define IF_DECLTYPE_AUTO_AVAILABLE(X) X
#else
# define IF_DECLTYPE_AUTO_AVAILABLE(X) /* NOTHING */
#endif

#define DO(Y)  { Y }
#define DO_(Y) /* NOTHING */

#define assert_type_of(V, T) assert((std::is_same<decltype(V), T>::value))

#define assert_auto_deduction(expr, by_auto) \
	{ \
		auto a = expr; assert_type_of(a         , by_auto); \
		               assert_type_of(tmpl(expr), by_auto); \
	}

#define assert_auto_lref_deduction(expr, by_auto_lref, _l) \
	DO##_l( \
		auto& al = expr; assert_type_of(al             , by_auto_lref); \
		                 assert_type_of(tmpl_lref(expr), by_auto_lref); \
	)

#define assert_auto_rref_deduction(expr, by_auto_rref) \
	{ \
		auto&& ar = expr; assert_type_of(ar             , by_auto_rref); \
		                  assert_type_of(tmpl_rref(expr), by_auto_rref); \
	}

#define assert_const_auto_deduction(expr, by_const_auto, by_const_tmpl) \
	{ \
		const auto ca = expr; assert_type_of(ca              , by_const_auto); \
		                      assert_type_of(const_tmpl(expr), by_const_tmpl); \
	}

#define assert_const_auto_lref_deduction(expr, by_const_auto_lref, _cl) \
	DO##_cl( \
		const auto& cal = expr; assert_type_of(cal                  , by_const_auto_lref); \
		                        assert_type_of(const_tmpl_lref(expr), by_const_auto_lref); \
	)

#define assert_const_auto_rref_deduction(expr, by_const_auto_rref, by_const_tmpl_rref, _cr) \
	DO##_cr( \
		const auto&& car = expr; assert_type_of(car                  , by_const_auto_rref); \
		                         assert_type_of(const_tmpl_rref(expr), by_const_tmpl_rref); \
	)

#define assert_category_and_deduction(expr, CAT, \
               by_decltype, \
			   by_auto, by_auto_lref, by_auto_rref, \
			   by_const_auto, by_const_auto_lref, by_const_auto_rref, \
			   by_const_tmpl, by_const_tmpl_rref, \
			   _l, _cl, _cr) \
	{ \
		assert(value_category_of(expr) == CAT); \
		assert_type_of(expr, by_decltype); \
		assert_auto_deduction(     expr, by_auto); \
		assert_auto_lref_deduction(expr, by_auto_lref, _l); \
		assert_auto_rref_deduction(expr, by_auto_rref); \
		assert_const_auto_deduction(     expr, by_const_auto, by_const_tmpl); \
		assert_const_auto_lref_deduction(expr, by_const_auto_lref, _cl); \
		assert_const_auto_rref_deduction(expr, by_const_auto_rref, by_const_tmpl_rref, _cr); \
		IF_DECLTYPE_AUTO_AVAILABLE( \
			decltype(auto)          da  = expr; assert_type_of(da  , by_decltype); \
			decltype(auto)&         dal = expr; assert_type_of(dal , by_decltype); \
			decltype(auto)&&        dar = expr; assert_type_of(dar , by_decltype); \
			const decltype(auto)   cda  = expr; assert_type_of(cda , by_decltype); \
			const decltype(auto)&  cdal = expr; assert_type_of(cdal, by_decltype); \
			const decltype(auto)&& cdar = expr; assert_type_of(cdar, by_decltype); \
		); \
	}

/*
	decltype:
		of unparenthesized id-expression: as declared
		of unparenthesized member access: as declared
		of lvalue: adds lvalue reference
		of xvalue: adds rvalue reference
		of prvalue
	auto: strips reference and top-level cv-qualification
	auto&:
		from lvalue: adds lvalue reference
		from const rvalue: adds lvalue reference
		from non-const rvalue: invalid
	auto&&:
		from lvalue: adds lvalue reference
		from rvalue: adds rvalue reference
*/
int main() {
	assert_category_and_deduction(42,
		PRVALUE,
		int,  // decltype: of prvalue
		int,    // auto: strips reference and top-level cv-qualification
		,       // auto&: from non-const rvalue: invalid
		int&&,  // auto&&: from rvalue: adds rvalue reference
		const int,    // const auto
		const int&,   // const auto&
		const int&&,  // const auto&&
		int,          // const T
		const int&&,  // const T&&
	_,,);

	assert_category_and_deduction("Hi!",
		LVALUE,
		const char(&)[4],  // decltype: of lvalue: adds lvalue reference
		const char*,       // auto: strips reference and top-level cv-qualification
		const char(&)[4],  // auto&: from lvalue: adds lvalue reference
		const char(&)[4],  // auto&&: from lvalue: adds lvalue reference
		const char* const,  // const auto
		const char(&)[4],   // const auto&
		,                   // const auto&&
		const char*,  // const T
		,             // const T&&
	,,_);

	int variable;
	assert_category_and_deduction(variable,
		LVALUE,
		int,  // decltype: of unparenthesized id-expression: as declared
		int,   // auto: strips reference and cv-qualification
		int&,  // auto&: from lvalue: adds lvalue reference
		int&,  // auto&&: from lvalue: adds lvalue reference
		const int,   // const auto
		const int&,  // const auto&
		,            // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	int& lvalref_variable = variable;
	assert_category_and_deduction(lvalref_variable,
		LVALUE,
		int&,  // decltype: of unparenthesized id-expression: as declared
		int,   // auto: strips reference and cv-qualification
		int&,  // auto&: from lvalue: adds lvalue reference
		int&,  // auto&&: from lvalue: adds lvalue reference
		const int,   // const auto
		const int&,  // const auto&
		,            // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	int&& rvalref_variable = static_cast<int&&>(variable);
	assert_category_and_deduction(rvalref_variable,
		LVALUE,
		int&&,  // decltype: of unparenthesized id-expression: as declared
		int,    // auto: strips reference and cv-qualification
		int&,   // auto&: from lvalue: adds lvalue reference
		int&,   // auto&&: from lvalue: adds lvalue reference
		const int,   // const auto
		const int&,  // const auto&
		,            // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	const int const_variable = variable;
	assert_category_and_deduction(const_variable,
		LVALUE,
		const int,  // decltype: of unparenthesized id-expression: as declared
		int,         // auto: strips reference and cv-qualification
		const int&,  // auto&: from lvalue: adds lvalue reference
		const int&,  // auto&&: from lvalue: adds lvalue reference
		const int,   // const auto
		const int&,  // const auto&
		,            // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	const int& const_lvalref_variable = variable;
	assert_category_and_deduction(const_lvalref_variable,
		LVALUE,
		const int&,  // decltype: of unparenthesized id-expression: as declared
		int,         // auto: strips reference and cv-qualification
		const int&,  // auto&: from lvalue: adds lvalue reference
		const int&,  // auto&&: from lvalue: adds lvalue reference
		const int,   // const auto
		const int&,  // const auto&
		,            // const auto&
		int,  // const T
		,     // const T&&
	,,_);

	const int&& const_rvalref_variable = static_cast<int&&>(variable);
	assert_category_and_deduction(const_rvalref_variable,
		LVALUE,
		const int&&,  // decltype: of unparenthesized id-expression: as declared
		int,         // auto: strips reference and cv-qualification
		const int&,  // auto&: from lvalue: adds lvalue reference
		const int&,  // auto&&: from lvalue: adds lvalue reference
		const int,   // const auto
		const int&,  // const auto&
		,            // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	assert_category_and_deduction(returns<int>(),
		PRVALUE,
		int,  // decltype: of prvalue
		int,    // auto: strips reference and cv-qualification
		,       // auto&: from non-const rvalue: invalid
		int&&,  // auto&&: from rvalue: adds rvalue reference
		const int,    // const auto
		const int&,   // const auto&
		const int&&,  // const auto&&
		int,          // const T
		const int&&,  // const T&&
	_,,);

	assert_category_and_deduction(returns<int&>(),
		LVALUE,
		int&,  // decltype: of lvalue: adds lvalue reference
		int,   // auto: strips reference and cv-qualification
		int&,  // auto&: from lvalue: adds lvalue reference
		int&,  // auto&&: from lvalue: adds lvalue reference
		const int,   // const auto
		const int&,  // const auto&
		,            // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	assert_category_and_deduction(returns<int&&>(),
		XVALUE,
		int&&,  // decltype: of xvalue: adds rvalue reference
		int,    // auto: strips reference and cv-qualification
		,       // auto&: from non-const rvalue: invalid
		int&&,  // auto&&: from rvalue: adds rvalue reference
		const int,    // const auto
		const int&,   // const auto&
		const int&&,  // const auto&&
		int,          // const T
		const int&&,  // const T&&
	_,,);

	// If T is a fundamental type, then a function returning const T is the same as returning T!
	assert_category_and_deduction(returns<const int>(),
		PRVALUE,
		int,  // decltype: of prvalue
		int,    // auto: strips reference and cv-qualification
		,       // auto&: from non-const rvalue: invalid
		int&&,  // auto&&: from rvalue: adds rvalue reference
		const int,    // const auto
		const int&,   // const auto &
		const int&&,  // const auto &&
		int,          // const T
		const int&&,  // const T&&
	_,,);

	assert_category_and_deduction(returns<const S>(),
		PRVALUE,
		const S,  // decltype: of prvalue
		S,          // auto: strips reference and cv-qualification
		const S&,   // auto&: from const rvalue: adds lvalue reference
		const S&&,  // auto&&: from rvalue: adds rvalue reference
		const S,    // const auto
		const S&,   // const auto&
		const S&&,  // const auto&&
		const S,    // const T
		const S&&,  // const T&&
	,,);

	assert_category_and_deduction(returns<const int&>(),
		LVALUE,
		const int&,  // decltype: of lvalue: adds lvalue reference
		int,         // auto: strips reference and cv-qualification
		const int&,  // auto&: from const rvalue: adds lvalue reference
		const int&,  // auto&&: from lvalue: adds lvalue reference
		const int,   // const auto
		const int&,  // const auto&
		,            // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	assert_category_and_deduction(returns<const int&&>(),
		XVALUE,
		const int&&,  // decltype: of xvalue: adds rvalue reference
		int,          // auto: strips reference and cv-qualification
		const int&,   // auto&: from const rvalue: adds lvalue reference
		const int&&,  // auto&&: from rvalue: adds rvalue reference
		const int,    // const auto
		const int&,   // const auto&
		const int&&,  // const auto&&
		int,          // const T
		const int&&,  // const T&&
	,,);

	assert_category_and_deduction(returns<int>,
		LVALUE,
		int(),  // decltype: of unparenthesized id-expression: as declared
		int(*)(),  // auto: strips reference and cv-qualification
		int(&)(),  // auto&: from lvalue: adds lvalue reference
		int(&)(),  // auto&&: from lvalue: adds lvalue reference
		int(*const)(),  // const auto
		int(&)(),       // const auto&
		int(&&)(),      // const auto&&
		int(*)(),  // const T
		int(&)(),  // const T&&
	,,);

	assert_category_and_deduction(&returns<int>,
		PRVALUE,
		int(*)(),  // decltype: of prvalue
		int(*)(),    // auto: strips reference and cv-qualification
		,            // auto&: from non-const rvalue: invalid
		int(*&&)(),  // auto&&: from rvalue: adds rvalue reference
		int(*const)(),    // const auto
		int(*const&)(),   // const auto&
		int(*const&&)(),  // const auto&&
		int(*)(),         // const T
		int(*const&&)(),  // const T&&
	_,,);

	assert_category_and_deduction(&S::f,
		PRVALUE,
		void(S::*)(char),  // decltype: of prvalue
		void(S::*)(char),    // auto: strips reference and cv-qualification
		,                    // auto&: from non-const rvalue: invalid
		void(S::*&&)(char),  // auto&&: from rvalue: adds rvalue reference
		void(S::*const)(char),    // const auto
		void(S::*const&)(char),   // const auto&
		void(S::*const&&)(char),  // const auto&&
		void(S::*)(char),         // const T
		void(S::*const&&)(char),  // const T&&
	_,,);

	S s;
	assert_category_and_deduction(s.i,
		LVALUE,
		volatile int,  // decltype: of unparenthesized member access: as declared
		int,            // auto: strips reference and cv-qualification
		volatile int&,  // auto&: from lvalue: adds lvalue reference
		volatile int&,  // auto&&: from lvalue: adds lvalue reference
		const int,            // const auto
		const volatile int&,  // const auto&
		,                     // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	assert_category_and_deduction((s.i),
		LVALUE,
		volatile int&,  // decltype: of lvalue: adds lvalue reference
		int,             // auto: strips reference and cv-qualification
		volatile int&,   // auto&: from lvalue: adds lvalue reference
		volatile int&,   // auto&&: from lvalue: adds lvalue reference
		const int,            // const auto
		const volatile int&,  // const auto&
		,                     // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	const S cs = s;
	assert_category_and_deduction(cs.i,
		LVALUE,
		volatile int,  // decltype: of unparenthesized member access: as declared
		int,                  // auto: strips reference and cv-qualification
		const volatile int&,  // auto&: from lvalue: adds lvalue reference
		const volatile int&,  // auto&&: from lvalue: adds lvalue reference
		const int,            // const auto
		const volatile int&,  // const auto&
		,                     // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	assert_category_and_deduction((cs.i),
		LVALUE,
		const volatile int&,  // decltype: of lvalue: adds lvalue reference
		int,                  // auto: strips reference and cv-qualification
		const volatile int&,  // auto&: from lvalue: adds lvalue reference
		const volatile int&,  // auto&&: from lvalue: adds lvalue reference
		const int,            // const auto
		const volatile int&,  // const auto&
		,                     // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	S&& rs = static_cast<S&&>(s);
	assert_category_and_deduction(rs.i,
		LVALUE,
		volatile int,  // decltype: of unparenthesized member access: as declared
		int,            // auto: strips reference and cv-qualification
		volatile int&,  // auto&: from lvalue: adds lvalue reference
		volatile int&,  // auto&&: from lvalue: adds lvalue reference
		const int,            // const auto
		const volatile int&,  // const auto&
		,                     // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	assert_category_and_deduction((rs.i),
		LVALUE,
		volatile int&,  // decltype: of lvalue: adds lvalue reference
		int,            // auto: strips reference and cv-qualification
		volatile int&,  // auto&: from lvalue: adds lvalue reference
		volatile int&,  // auto&&: from lvalue: adds lvalue reference
		const int,            // const auto
		const volatile int&,  // const auto&
		,                     // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	assert_category_and_deduction(static_cast<S&&>(s).i,
		XVALUE,
		volatile int,  // decltype: of unparenthesized member access: as declared
		int,             // auto: strips reference and cv-qualification
		,                // auto&: from non-const rvalue: invalid
		volatile int&&,  // auto&&: from rvalue: adds rvalue reference
		const int,             // const auto
		,                      // const auto&
		const volatile int&&,  // const auto&&
		int,                   // const T
		const volatile int&&,  // const T&&
	_,_,);

	assert_category_and_deduction((static_cast<S&&>(s).i),
		XVALUE,
		volatile int&&,  // decltype: of xvalue: adds rvalue reference
		int,             // auto: strips reference and cv-qualification
		,                // auto&: from non-const rvalue: invalid
		volatile int&&,  // auto&&: from rvalue: adds rvalue reference
		const int,             // const auto
		,                      // const auto&
		const volatile int&&,  // const auto&&
		int,                   // const T
		const volatile int&&,  // const T&&
	_,_,);

	assert_category_and_deduction(returns<bool>() ? s.i : cs.i,
		PRVALUE,
		int,  // decltype: of prvalue
		int,    // auto: strips reference and cv-qualification
		,       // auto&: from non-const rvalue: invalid
		int&&,  // auto&&: from rvalue: adds rvalue reference
		const int,    // const auto
		const int&,   // const auto&
		const int&&,  // const auto&&
		int,          // const T
		const int&&,  // const T&&
	_,,);

	assert_category_and_deduction(returns<bool>() ? cs.i : cs.i,
		LVALUE,
		const volatile int&,  // decltype: of lvalue: adds lvalue reference
		int,                  // auto: strips reference and cv-qualification
		const volatile int&,  // auto&: from lvalue: adds lvalue reference
		const volatile int&,  // auto&&: from lvalue: adds lvalue reference
		const int,            // const auto
		const volatile int&,  // const auto&
		,                     // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	assert_category_and_deduction(returns<bool>() ? rvalref_variable : rvalref_variable,
		LVALUE,
		int&,  // decltype: of lvalue: adds lvalue reference
		int,   // auto: strips reference and cv-qualification
		int&,  // auto&: from lvalue: adds lvalue reference
		int&,  // auto&&: from lvalue: adds lvalue reference
		const int,   // const auto
		const int&,  // const auto&
		,            // const auto&&
		int,  // const T
		,     // const T&&
	,,_);

	assert_category_and_deduction(returns<bool>() ? rvalref_variable : const_rvalref_variable,
		PRVALUE,
		int,  // decltype: of prvalue
		int,    // auto: strips reference and cv-qualification
		,       // auto&: from non-const rvalue: invalid
		int&&,  // auto&&: from rvalue: adds rvalue reference
		const int,    // const auto
		const int&,   // const auto&
		const int&&,  // const auto&&
		int,          // const T
		const int&&,  // const T&&
	_,,);

	assert_category_and_deduction(static_cast<int&&>(variable),
		XVALUE,
		int&&,  // decltype: of xvalue: adds rvalue reference
		int,    // auto: strips reference and cv-qualification
		,       // auto&: from non-const rvalue: invalid
		int&&,  // auto&&: from rvalue: adds rvalue reference
		const int,    // const auto
		const int&,   // const auto&
		const int&&,  // const auto&&
		int,          // const T
		const int&&,  // const T&&
	_,,);
}
