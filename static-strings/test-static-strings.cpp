#include "static-strings.hpp"
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


using E = static_string::from_provider<EMPTY>;
using H = static_string::from_provider<Hello>;
using W = static_string::from_provider<World>;


void testBasics() {
	assert(E::size == 0);
	assert(E::string() == "");

	assert(H::size == 6);
	assert(H::string() == "Hello ");

	assert(W::size == 5);
	assert(W::string() == "World");
}


void testConcat() {
	using HW = static_string::concat<H, W>;
	using X = static_string::concat<H, W, static_string::from_provider<Exclamation>>;

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

	using W   = static_string::from_provider<   World>;
	using WW  = static_string::from_provider<  WWorld>;
	using W16 = static_string::from_provider<U16World>;
	using W32 = static_string::from_provider<U32World>;

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


void testExplicitSize() {
	struct World {
		enum { size = 11 };
		constexpr static const char* str() {
			return  "Hello\0World";
		}
	};

	using HW1 = static_string::from_provider<World>;
	using HW2 = static_string::from_provider<World, 7>;

	assert(HW1::size == 11);
	assert(HW2::size == 7);

	assert(HW1::string() == std::string("Hello") + '\0' + "World");
	assert(HW2::string() == std::string("Hello") + '\0' + 'W');
}


int main() {
	testBasics();
	testConcat();
	testCharTypes();
	testExplicitSize();
}
