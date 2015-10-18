#include "static-strings.hpp"
#include <cassert>
#include <type_traits>


void testBuildingWithIndividualChars() {
	using Empty = static_string::static_string<char>;
	assert(Empty::size == 0);
	assert(Empty::string() == "");

	using Hello = static_string::static_string<char, 'H', 'e', 'l', 'l', 'o'>;
	assert(Hello::size == 5);
	assert(Hello::string() == "Hello");
}


void testBuildingWithStringProviders() {
	struct EmptyStrProvider {
		constexpr static const char* str() {
			return "";
		}
	};
	using Empty = static_string::from_provider<EmptyStrProvider>;
	assert(Empty::size == 0);
	assert(Empty::string() == "");

	struct HelloStrProvider {
		constexpr static const char* str() {
			return "Hello";
		}
	};
	using Hello = static_string::from_provider<HelloStrProvider>;
	assert(Hello::size == 5);
	assert(Hello::string() == "Hello");
}


void testConcat() {
	struct EmptyStrProvider { constexpr static const char* str() { return ""; } };
	struct WorldStrProvider { constexpr static const char* str() { return "World"; } };

	using Hello = static_string::static_string<char, 'H', 'e', 'l', 'l', 'o', ' '>;
	using Empty = static_string::from_provider<EmptyStrProvider>;
	using World = static_string::from_provider<WorldStrProvider>;

	using HelloWorld1 = static_string::concat<Hello, World>;
	assert(HelloWorld1::size == 11);
	assert(HelloWorld1::string() == "Hello World");

	using HelloWorld2 = static_string::concat<Hello, Empty, World>;
	assert(HelloWorld2::size == 11);
	assert(HelloWorld2::string() == "Hello World");
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


void testStringProviderWithExplicitSize() {
	struct World {
		enum { size = 11 };
		constexpr static const char* str() {
			return  "Hello\0World";
		}
	};

	using HelloWorld = static_string::from_provider<World>;
	using HelloW     = static_string::from_provider<World, 7>;

	assert(HelloWorld::size == 11);
	assert(    HelloW::size == 7);

	assert(HelloWorld::string() == std::string("Hello") + '\0' + "World");
	assert(    HelloW::string() == std::string("Hello") + '\0' + 'W');
}


void testCharAt() {
	using Hello = static_string::static_string<char, 'H', 'e', 'l', 'l', 'o'>;

	assert(Hello::char_at<0>::value == 'H');
	assert(Hello::char_at<1>::value == 'e');
	assert(Hello::char_at<2>::value == 'l');
	assert(Hello::char_at<3>::value == 'l');
	assert(Hello::char_at<4>::value == 'o');
	//(void)Hello::char_at<5>::value; // SHOULD NOT COMPILE!
}


void testFind() {
	using Empty = static_string::static_string<char>;
	assert(Empty::find<'a'>::value == static_string::NOT_FOUND);

	using abacab = static_string::static_string<char, 'a', 'b', 'a', 'c', 'a', 'b'>;
	assert(abacab::find<'a'>::value == 0);
	assert(abacab::find<'b'>::value == 1);
	assert(abacab::find<'c'>::value == 3);
	assert(abacab::find<'d'>::value == static_string::NOT_FOUND);
}


int main() {
	testBuildingWithIndividualChars();
	testBuildingWithStringProviders();
	testConcat();
	testCharTypes();
	testStringProviderWithExplicitSize();
	testCharAt();
	testFind();
}
