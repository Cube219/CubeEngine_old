#pragma once

#include <string>
#include "format.h"

#define STR_UTF16

namespace cube
{
	using U8String = std::basic_string<char>;
	using UCS2String = std::basic_string<unsigned short>;
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

	std::string ToASCIIString(const U8String& str);
	std::string ToASCIIString(const UCS2String& str);
	std::string ToASCIIString(const U16String& str);
	std::string ToASCIIString(const U32String& str);

	U8String ToU8StringFromASCII(const std::string& str);
	U8String ToU8String(const UCS2String& str);
	U8String ToU8String(const U16String& str);
	U8String ToU8String(const U32String& str);

	UCS2String ToUCS2StringFromASCII(const std::string& str);
	UCS2String ToUCS2String(const U8String& str);
	UCS2String ToUCS2String(const U16String& str);
	UCS2String ToUCS2String(const U32String& str);

	U16String ToU16StringFromASCII(const std::string& str);
	U16String ToU16String(const U8String& str);
	U16String ToU16String(const UCS2String& str);
	U16String ToU16String(const U32String& str);

	U32String ToU32StringFromASCII(const std::string& str);
	U32String ToU32String(const U8String& str);
	U32String ToU32String(const UCS2String& str);
	U32String ToU32String(const U16String& str);

	namespace internal
	{
		// Helper functions
		char32_t GetUTF8CharAndMove(U8String::const_iterator& iter);
		int GetUTF8CharSize(U8String::const_iterator iter);
		int GetUTF8RequiredCharSize(char32_t ch);
		void InsertCharInUTF8(U8String& str, char32_t ch);

		char32_t GetUTF16CharAndMove(U16String::const_iterator& iter);
		int GetUTF16CharSize(U16String::const_iterator iter);
		int GetUTF16RequiredCharSize(char32_t ch);
		void InsertCharInUTF16(U16String& str, char32_t ch);
	} // namespace internal

#if defined (STR_UTF8)

	using Character = char;
	using String = U8String;
	#define CUBE_T(text) u8 ## text
	inline String ToStringFromASCII(const std::string& str) { return ToU8StringFromASCII(str); }
	inline String ToString(const U8String& str) { return str; }
	inline String ToString(const UCS2String& str) { return ToU8String(str); }
	inline String ToString(const U16String& str) { return ToU8String(str); }
	inline String ToString(const U32String& str) { return ToU8String(str); }

#elif defined (STR_UCS2)

	using Character = unsigned short;
	using String = UCS2String;
	#define CUBE_T(text) (const unsigned short*)u ## text
	inline String ToStringFromASCII(const std::string& str) { return ToUCS2StringFromASCII(str); }
	inline String ToString(const U8String& str) { return ToUCS2String(str); }
	inline String ToString(const UCS2String& str) { return str; }
	inline String ToString(const U16String& str) { return ToUCS2String(str); }
	inline String ToString(const U32String& str) { return ToUCS2String(str); }

#elif defined (STR_UTF16)

	using Character = char16_t;
	using String = U16String;
	#define CUBE_T(text) u ## text
	inline String ToStringFromASCII(const std::string& str) { return ToU16StringFromASCII(str); }
	inline String ToString(const U8String& str) { return ToU16String(str); }
	inline String ToString(const UCS2String& str) { return ToU16String(str); }
	inline String ToString(const U16String& str) { return str; }
	inline String ToString(const U32String& str) { return ToU16String(str); }

#elif defined (STR_UTF32)

	using Character = char32_t;
	using String = U32String;
	#define CUBE_T(text) U ## text
	inline String ToStringFromASCII(const std::string& str) { return ToU32StringFromASCII(str); }
	inline String ToString(const U8String& str) { return ToU32String(str); }
	inline String ToString(const U16String& str) { return ToU32String(str); }
	inline String ToString(const UCS2String& str) { return ToU32String(str); }
	inline String ToString(const U32String& str) { return str; }

#else
	#error You must define one of string type
#endif
} // namespace cube

// Formatting each strings
namespace fmt
{
	namespace internal
	{
		// U8String
		inline void format_arg(fmt::BasicFormatter<unsigned short>& f, const unsigned short* format_str, const cube::U8String& s)
		{
			f.writer().write(cube::ToUCS2String(s));
		}
		inline void format_arg(fmt::BasicFormatter<char16_t>& f, const char16_t* format_str, const cube::U8String& s)
		{
			f.writer().write(cube::ToU16String(s));
		}
		inline void format_arg(fmt::BasicFormatter<char32_t>& f, const char32_t* format_str, const cube::U8String& s)
		{
			f.writer().write(cube::ToU32String(s));
		}

		// UCS2String
		inline void format_arg(fmt::BasicFormatter<char>& f, const char* format_str, const cube::UCS2String& s)
		{
			f.writer().write(cube::ToU8String(s));
		}
		inline void format_arg(fmt::BasicFormatter<unsigned short>& f, const unsigned short* format_str, const cube::UCS2String& s)
		{
			f.writer().write(s);
		}
		inline void format_arg(fmt::BasicFormatter<char16_t>& f, const char16_t* format_str, const cube::UCS2String& s)
		{
			f.writer().write(cube::ToU16String(s));
		}
		inline void format_arg(fmt::BasicFormatter<char32_t>& f, const char32_t* format_str, const cube::UCS2String& s)
		{
			f.writer().write(cube::ToU32String(s));
		}

		// U16String
		inline void format_arg(fmt::BasicFormatter<char>& f, const char* format_str, const cube::U16String& s)
		{
			f.writer().write(cube::ToU8String(s));
		}
		inline void format_arg(fmt::BasicFormatter<unsigned short>& f, const unsigned short* format_str, const cube::U16String& s)
		{
			f.writer().write(cube::ToUCS2String(s));
		}
		inline void format_arg(fmt::BasicFormatter<char16_t>& f, const char16_t* format_str, const cube::U16String& s)
		{
			f.writer().write(s);
		}
		inline void format_arg(fmt::BasicFormatter<char32_t>& f, const char32_t* format_str, const cube::U16String& s)
		{
			f.writer().write(cube::ToU32String(s));
		}

		// U32String
		inline void format_arg(fmt::BasicFormatter<char>& f, const char* format_str, const cube::U32String& s)
		{
			f.writer().write(cube::ToU8String(s));
		}
		inline void format_arg(fmt::BasicFormatter<unsigned short>& f, const unsigned short* format_str, const cube::U32String& s)
		{
			f.writer().write(cube::ToUCS2String(s));
		}
		inline void format_arg(fmt::BasicFormatter<char16_t>& f, const char16_t* format_str, const cube::U32String& s)
		{
			f.writer().write(cube::ToU16String(s));
		}
		inline void format_arg(fmt::BasicFormatter<char32_t>& f, const char32_t* format_str, const cube::U32String& s)
		{
			f.writer().write(s);
		}
	} // namespace internal
} // namespace fmt