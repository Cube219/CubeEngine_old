#pragma once

#include <string>

#define STR_UTF16

namespace cube
{
	using U8String = std::basic_string<char>;
	using UCS2String = std::basic_string<short>;
	using U16String = std::basic_string<char16_t>;
	using U32String = std::basic_string<char32_t>;

	inline void StringMoveNext(U8String::iterator& iter, size_t offset)
	{
		while(offset > 0) {
			char ch = *iter;
			if((unsigned char)ch <= 127)
				iter += 1;
			else if((ch & 0xE0) == 0xC0)
				iter += 2;
			else if((ch & 0xF0) == 0xE0)
				iter += 3;
			else if((ch & 0xF8) == 0xF0)
				iter += 4;
			else
				throw;

			offset--;
}
	}
	inline void StringMovePrev(U8String::iterator& iter, size_t offset)
	{
		while(offset > 0) {
			while(1) {
				iter--;

				char ch = *iter;
				if((ch & 0xC0) != 0x80)
					break;
			}

			offset--;
		}
	}

	inline void StringMoveNext(UCS2String::iterator& iter, size_t offset)
	{
		iter += offset;
	}
	inline void StringMovePrev(UCS2String::iterator& iter, size_t offset)
	{
		iter -= offset;
	}

	inline void StringMoveNext(U16String::iterator& iter, size_t offset)
	{
		while(offset > 0) {
			char16_t ch = *iter;

			if((ch & 0xF800) == 0xD800)
				iter += 2;
			else
				iter += 1;

			offset--;
		}
	}
	inline void StringMovePrev(U16String::iterator& iter, size_t offset)
	{
		while(offset > 0) {
			iter--;

			char16_t ch = *iter;
			if((ch & 0xF800) == 0xD800)
				iter--;

			offset--;
		}
	}

	inline void StringMoveNext(U32String::iterator& iter, size_t offset)
	{
		iter += offset;
	}
	inline void StringMovePrev(U32String::iterator& iter, size_t offset)
	{
		iter -= offset;
	}

	inline std::string ToASCIIString(const U8String& str)
	{
		return std::string();
	}
	inline std::string ToASCIIString(const UCS2String& str)
	{
		return std::string();
	}
	inline std::string ToASCIIString(const U16String& str)
	{
		return std::string();
	}
	inline std::string ToASCIIString(const U32String& str)
	{
		return std::string();
	}

#if defined (STR_UTF8)

	using String = U8String;
	#define CUBE_T(text) u8 ## text

#elif defined (STR_UCS2)

	using String = UCS2String;
	#define CUBE_T(text) (const short*)u ## text

#elif defined (STR_UTF16)

	using String2 = U16String;
	#define CUBE_T(text) u ## text

#elif defined (STR_UTF32)

	using String = U32String;
	#define CUBE_T(text) U ## text

#else
	#error You must define one of string type
#endif
}