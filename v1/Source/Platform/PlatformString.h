#pragma once

#include "PlatformHeader.h"

namespace cube
{
#ifdef _WIN32
	using PCharacter = wchar_t;
	using PString = std::wstring;
#endif

	PLATFORM_EXPORT PString ToPString(const U8String& str);
	PLATFORM_EXPORT PString ToPString(const UCS2String& str);
	PLATFORM_EXPORT PString ToPString(const U16String& str);
	PLATFORM_EXPORT PString ToPString(const U32String& str);

	PLATFORM_EXPORT U8String ToU8String(const PString& str);
	PLATFORM_EXPORT UCS2String ToUCS2String(const PString& str);
	PLATFORM_EXPORT U16String ToU16String(const PString& str);
	PLATFORM_EXPORT U32String ToU32String(const PString& str);
} // namespace cube
