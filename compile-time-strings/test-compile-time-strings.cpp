#include "compile-time-strings.hpp"
#include <cassert>
#include <type_traits>

struct EMPTY {
	constexpr static const char* str() {
		return "";
	}
};

struct Hello {
	constexpr static const char* str() {
		return "Hello ";
	}
};

struct World {
	constexpr static const char* str() {
		return "World";
	}
};

struct Exclamation {
	constexpr static const char* str() {
		return "!";
	}
};


using E = ctstring::ctstring<EMPTY>;
using H = ctstring::ctstring<Hello>;
using W = ctstring::ctstring<World>;


void testBasics() {
	assert(E::size == 0);
	assert(E::string() == "");

	assert(H::size == 6);
	assert(H::string() == "Hello ");

	assert(W::size == 5);
	assert(W::string() == "World");
}


void testConcat() {
	using HW = ctstring::concat<H, W>;
	using X = ctstring::concat<H, W, ctstring::ctstring<Exclamation>>;

	assert(HW::size == 11);
	assert(HW::string() == "Hello World");

	assert(X::size == 12);
	assert(X::string() == "Hello World!");
}


void testCharTypes() {
	struct    World { constexpr static const     char* str() { return  "World"; } };
	struct   WWorld { constexpr static const  wchar_t* str() { return L"World"; } };
	struct U16World { constexpr static const char16_t* str() { return u"World"; } };
	struct U32World { constexpr static const char32_t* str() { return U"World"; } };

	using W   = ctstring::ctstring<   World>;
	using WW  = ctstring::ctstring<  WWorld>;
	using W16 = ctstring::ctstring<U16World>;
	using W32 = ctstring::ctstring<U32World>;

	assert((std::is_same<decltype(  W::string()),    std::string>::value));
	assert((std::is_same<decltype( WW::string()),   std::wstring>::value));
	assert((std::is_same<decltype(W16::string()), std::u16string>::value));
	assert((std::is_same<decltype(W32::string()), std::u32string>::value));

	assert(  W::string() ==  "World");
	assert( WW::string() == L"World");
	assert(W16::string() == u"World");
	assert(W32::string() == U"World");

	assert((std::is_same<  W::char_type,     char>::value));
	assert((std::is_same< WW::char_type,  wchar_t>::value));
	assert((std::is_same<W16::char_type, char16_t>::value));
	assert((std::is_same<W32::char_type, char32_t>::value));

	assert((std::is_same<  W::string_type,    std::string>::value));
	assert((std::is_same< WW::string_type,   std::wstring>::value));
	assert((std::is_same<W16::string_type, std::u16string>::value));
	assert((std::is_same<W32::string_type, std::u32string>::value));
}


int main() {
	testBasics();
	testConcat();
	testCharTypes();
}
