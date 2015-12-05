#include <cassert>


enum CV {
	NONCV, CONST, VOLATILE, CONST_VOLATILE
};

struct CVQualifications {
	CV noncv_not_overloaded() { return NONCV; }

	CV const_not_overloaded() const { return CONST; }

	CV volatile_not_overloaded () volatile { return VOLATILE; }

	CV constvolatile_not_overloaded () const volatile { return CONST_VOLATILE; }

	CV noncv_vs_const()       { return NONCV; }
	CV noncv_vs_const() const { return CONST; }

	CV noncv_vs_volatile()          { return NONCV; }
	CV noncv_vs_volatile() volatile { return VOLATILE; }

	CV noncv_vs_constvolatile()                { return NONCV; }
	CV noncv_vs_constvolatile() const volatile { return CONST_VOLATILE; }

	CV const_vs_volatile() const    { return CONST; }
	CV const_vs_volatile() volatile { return VOLATILE; }

	CV const_vs_constvolatile() const          { return CONST; }
	CV const_vs_constvolatile() const volatile { return CONST_VOLATILE; }

	CV volatile_vs_constvolatile()       volatile { return VOLATILE; }
	CV volatile_vs_constvolatile() const volatile { return CONST_VOLATILE; }
};

void cv() {
	               CVQualifications          noncv_instance;
	const          CVQualifications          const_instance = noncv_instance;
	      volatile CVQualifications       volatile_instance;
	const volatile CVQualifications const_volatile_instance = noncv_instance;

	assert(         noncv_instance.noncv_not_overloaded() == NONCV);
	//assert(         const_instance.noncv_not_overloaded() == NONCV);  // Const          instance can call only const          methods
	//assert(      volatile_instance.noncv_not_overloaded() == NONCV);  // Volatile       instance can call only volatile       methods
	//assert(const_volatile_instance.noncv_not_overloaded() == NONCV);  // Const volatile instance can call only const volatile methods

	assert(         noncv_instance.const_not_overloaded() == CONST);
	assert(         const_instance.const_not_overloaded() == CONST);
	//assert(      volatile_instance.const_not_overloaded() == CONST);  // Volatile       instance can call only volatile       methods
	//assert(const_volatile_instance.const_not_overloaded() == CONST);  // Const volatile instance can call only const volatile methods

	assert(         noncv_instance.volatile_not_overloaded() == VOLATILE);
	//assert(         const_instance.volatile_not_overloaded() == VOLATILE);  // Const          instance can call only const          methods
	assert(      volatile_instance.volatile_not_overloaded() == VOLATILE);
	//assert(const_volatile_instance.volatile_not_overloaded() == VOLATILE);  // Const volatile instance can call only const volatile methods

	assert(         noncv_instance.constvolatile_not_overloaded() == CONST_VOLATILE);
	assert(         const_instance.constvolatile_not_overloaded() == CONST_VOLATILE);
	assert(      volatile_instance.constvolatile_not_overloaded() == CONST_VOLATILE);
	assert(const_volatile_instance.constvolatile_not_overloaded() == CONST_VOLATILE);

	assert(         noncv_instance.noncv_vs_const() == NONCV);
	assert(         const_instance.noncv_vs_const() == CONST);
	//assert(      volatile_instance.noncv_vs_const() == ?);  // Volatile       instance can call only volatile       methods
	//assert(const_volatile_instance.noncv_vs_const() == ?);  // Const volatile instance can call only const volatile methods

	assert(         noncv_instance.noncv_vs_volatile() == NONCV);
	//assert(         const_instance.noncv_vs_volatile() == ?);  // Const          instance can call only const          methods
	assert(      volatile_instance.noncv_vs_volatile() == VOLATILE);
	//assert(const_volatile_instance.noncv_vs_volatile() == ?);  // Const volatile instance can call only const volatile methods

	assert(         noncv_instance.noncv_vs_constvolatile() == NONCV);  // Less qualified method overload is prefered
	assert(         const_instance.noncv_vs_constvolatile() == CONST_VOLATILE);
	assert(      volatile_instance.noncv_vs_constvolatile() == CONST_VOLATILE);
	assert(const_volatile_instance.noncv_vs_constvolatile() == CONST_VOLATILE);

	//assert(         noncv_instance.const_vs_volatile() == ?);  // Ambiguity
	assert(         const_instance.const_vs_volatile() == CONST);
	assert(      volatile_instance.const_vs_volatile() == VOLATILE);
	//assert(const_volatile_instance.const_vs_volatile() == 0);  // Const volatile instance can call only const volatile methods

	assert(         noncv_instance.const_vs_constvolatile() == CONST);  // Less qualified method overload is prefered
	assert(         const_instance.const_vs_constvolatile() == CONST);  // Less qualified method overload is prefered
	assert(      volatile_instance.const_vs_constvolatile() == CONST_VOLATILE);
	assert(const_volatile_instance.const_vs_constvolatile() == CONST_VOLATILE);

	assert(         noncv_instance.volatile_vs_constvolatile() == VOLATILE);  // Less qualified method overload is prefered
	assert(         const_instance.volatile_vs_constvolatile() == CONST_VOLATILE);
	assert(      volatile_instance.volatile_vs_constvolatile() == VOLATILE);  // Less qualified method overload is prefered
	assert(const_volatile_instance.volatile_vs_constvolatile() == CONST_VOLATILE);
}


enum REF { NONREF, LVALREF, RVALREF };

struct REFQualifications {
	REF nonref_not_overloaded() { return NONREF; }

	REF lvalref_not_overloaded() & { return LVALREF; }

	REF rvalref_not_overloaded() && { return RVALREF; }

	/*
	REF nonref_vs_lvalref()   { return NONREF; }
	REF nonref_vs_lvalref() & { return LVALREF; }  // L-value-ref method cannot be overloaded with non-ref-qualified method
	*/

	/*
	REF nonref_vs_rvalref()    { return NONREF; }
	REF nonref_vs_rvalref() && { return RVALREF; }  // R-value-ref method cannot be overloaded with non-ref-qualified method
	*/

	REF lvalref_vs_rvalref() &  { return LVALREF; }
	REF lvalref_vs_rvalref() && { return RVALREF; }
};

REFQualifications expression() { return {}; }

void ref() {
	REFQualifications lvalue;
	#define rvalue expression()

	assert(lvalue.nonref_not_overloaded() == NONREF);
	assert(rvalue.nonref_not_overloaded() == NONREF);

	assert(lvalue.lvalref_not_overloaded() == LVALREF);
	//assert(rvalue.lvalref_not_overloaded() == LVALREF);  // L-value-ref method can only be called on lvalue instances

	//assert(lvalue.rvalref_not_overloaded() == RVALREF);  // R-value-ref method can only be called on rvalue instances
	assert(rvalue.rvalref_not_overloaded() == RVALREF);

	/*
	assert(lvalue.nonref_vs_lvalref() == ?);
	assert(rvalue.nonref_vs_lvalref() == ?);
	*/

	/*
	assert(lvalue.nonref_vs_rvalref() == ?);
	assert(rvalue.nonref_vs_rvalref() == ?);
	*/

	assert(lvalue.lvalref_vs_rvalref() == LVALREF);
	assert(rvalue.lvalref_vs_rvalref() == RVALREF);
}


int main() {
	cv();
	ref();
}
