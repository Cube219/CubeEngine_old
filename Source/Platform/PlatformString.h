#pragma once

#include "Base/String.h"

namespace cube
{
#ifdef _WIN32
	using PString = std::wstring;
#endif

	PString ToPString(const U8String& str);
	PString ToPString(const UCS2String& str);
	PString ToPString(const U16String& str);
	PString ToPString(const U32String& str);

	U8String ToU8String(const PString& str);
	UCS2String ToUCS2String(const PString& str);
	U16String ToU16String(const PString& str);
	U32String ToU32String(const PString& str);

	void PrintToConsole(const U8String& str);
	void PrintToConsole(const UCS2String& str);
	void PrintToConsole(const U16String& str);
	void PrintToConsole(const U32String& str);
} // namespace cube
