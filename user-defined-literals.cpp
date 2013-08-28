#include <iostream>
#include <string>


template <unsigned long long N, char...BB>
struct binary_literal {
	enum { value = N };
};

template <unsigned long long N, char B, char...BB>
struct binary_literal<N, B, BB...> {
	static_assert(B == '0' || B == '1', "invalid binary digit");
	enum {
		value = binary_literal<(N << 1) | (B - '0'), BB...>::value
	};
};

template <char...BB> constexpr          int       operator"" _b()    { return binary_literal<0, BB...>::value; }
template <char...BB> constexpr unsigned int       operator"" _bU()   { return binary_literal<0, BB...>::value; }
template <char...BB> constexpr          long      operator"" _bL()   { return binary_literal<0, BB...>::value; }
template <char...BB> constexpr unsigned long      operator"" _bUL()  { return binary_literal<0, BB...>::value; }
template <char...BB> constexpr          long long operator"" _bLL()  { return binary_literal<0, BB...>::value; }
template <char...BB> constexpr unsigned long long operator"" _bULL() { return binary_literal<0, BB...>::value; }


unsigned long long operator"" _bULL(const char* s, size_t sz) {
	unsigned long long value = 0;
	for(size_t i = 0; i < sz; i++) {
		char ch = s[i];
		if(ch == '0') {
			value = (value << 1);
		} else if(ch == '1') {
			value = (value << 1) | 1;
		} else if(ch != '_') {
			throw "Not a binary digit";
		}
	}
	return value;
}
long long     operator"" _bLL(const char* s, size_t sz) { return operator"" _bULL(s, sz); }
unsigned long operator"" _bUL(const char* s, size_t sz) { return operator"" _bULL(s, sz); }
long          operator"" _bL(const char* s, size_t sz)  { return operator"" _bULL(s, sz); }
unsigned int  operator"" _bU(const char* s, size_t sz)  { return operator"" _bULL(s, sz); }
int           operator"" _b(const char* s, size_t sz)   { return operator"" _bULL(s, sz); }


std::string operator"" _s(const char* s, size_t sz) {
	return std::string(s, s+sz);
}


using namespace std;

int main() {
	cout <<           0_b << endl;
	cout <<           1_b << endl;
	cout <<          10_b << endl;
	cout <<          11_b << endl;
	cout <<        0100_b << endl;
	cout <<        0101_b << endl;
	cout <<        0110_b << endl;
	cout <<        0111_b << endl;
	cout <<        1000_b << endl;
	cout <<        1001_b << endl;
	cout <<        1010_b << endl;
	cout <<        1011_b << endl;
	cout <<        1100_b << endl;
	cout <<        1101_b << endl;
	cout <<        1110_b << endl;
	cout <<        1111_b << endl;
	cout <<       10000_b << endl;
	cout <<      100000_b << endl;
	cout <<     1000000_b << endl;
	cout <<    10000000_b << endl;
	cout <<   100000000_b << endl;
	cout <<  1000000000_b << endl;
	cout << 10000000000_b << endl;
	cout         <<          endl;
	
	cout << -"1101_0100__0011_0001"_b << endl;
	cout <<  "1101_0100__0011_0001"_b << endl;
	cout << endl;
	
	cout << string("abc\0").length() << endl;
	cout << "abc\0"_s.length() << endl;
	cout << endl;
}
