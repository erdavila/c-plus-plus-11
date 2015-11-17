#include <cassert>
#include <type_traits>


#define assert_same(A, B)     assert(( std::is_same<A, B>()));
#define assert_NOT_same(A, B) assert((!std::is_same<A, B>()));


/*
 * At 8.3.2/6 [dcl.ref]:
 *
 * If a typedef-name (7.1.3, 14.1) or a decltype-specifier (7.1.6.2) denotes a
 * type TR that is a reference to a type T, an attempt to create the type “lvalue
 * reference to cv TR” creates the type “lvalue reference to T”, while an attempt
 * to create the type “rvalue reference to cv TR” creates the type TR.
*/

template <typename T, typename TR>
void create_lvalue_reference_to_cv_TR() {
	/*
	 * ... an attempt to create the type “lvalue reference to cv TR” creates the
	 * type “lvalue reference to T”, ...
	 *
	 * cv  TR   &  ->  T&
	 * ------------------
	 * cv (T&)  &  ->  T&
	 * cv (T&&) &  ->  T&
	 */
	assert_same(               TR&, T&);
	assert_same(const          TR&, T&);
	assert_same(      volatile TR&, T&);
	assert_same(const volatile TR&, T&);
}

template <typename T, typename TR>
void create_rvalue_reference_to_cv_TR() {
	/*
	 * ... while an attempt to create the type “rvalue reference to cv TR”
	 * creates the type TR.
	 *
	 * cv  TR   &&  ->  TR
	 * --------------------
	 * cv (T&)  &&  ->  T&
	 * cv (T&&) &&  ->  T&&
	 */
	assert_same(               TR&&, TR);
	assert_same(const          TR&&, TR);
	assert_same(      volatile TR&&, TR);
	assert_same(const volatile TR&&, TR);
}


/*
 * At 8.3.2/1 [dcl.ref]:
 *
 * Cv-qualified references are ill-formed except when the cv-qualifiers are
 * introduced through the use of a typedef-name (7.1.3, 14.1) or decltype-specifier
 * (7.1.6.2), in which case the cv-qualifiers are ignored.
 */
template <typename T, typename TR>
void create_cv_TR() {
	/*
	 * ... the cv-qualifiers are ignored.
	 *
	 * cv  TR    ->  TR
	 * -----------------
	 * cv (T&)   ->  T&
	 * cv (T&&)  ->  T&&
	 */
	assert_same(const          TR, TR);
	assert_same(      volatile TR, TR);
	assert_same(const volatile TR, TR);
}


template <typename T>
void create_lvalue_reference_to_cv_T() {
	assert_NOT_same(const          T&, T&);
	assert_NOT_same(      volatile T&, T&);
	assert_NOT_same(const volatile T&, T&);
}

template <typename T>
void create_rvalue_reference_to_cv_T() {
	assert_NOT_same(const          T&&, T&&);
	assert_NOT_same(      volatile T&&, T&&);
	assert_NOT_same(const volatile T&&, T&&);
}


template <typename T, typename TR>
void create_references() {
	create_lvalue_reference_to_cv_TR<T, TR>();
	create_rvalue_reference_to_cv_TR<T, TR>();
	create_cv_TR<T, TR>();
	create_lvalue_reference_to_cv_T<T>();
	create_rvalue_reference_to_cv_T<T>();
}

int main() {
	create_references<int, int&>();
	create_references<int, int&&>();
}
