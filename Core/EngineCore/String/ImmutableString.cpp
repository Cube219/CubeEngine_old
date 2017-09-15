#include "ImmutableString.h"

namespace cube
{
	namespace core
	{
		ImmutableString::ImmutableString()
		{
		}

		ImmutableString::ImmutableString(WString& string)
		{
			mStringData = string;
		}

		ImmutableString::ImmutableString(wchar_t* string)
		{
			mStringData = WString(string);
		}

		ImmutableString::~ImmutableString()
		{
		}
	}
}