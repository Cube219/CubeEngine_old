#pragma once

#include "..\EngineCoreHeader.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT ImmutableString
		{
		public:
			ImmutableString();
			ImmutableString(WString& string);
			ImmutableString(wchar_t* string);
			~ImmutableString();

			ImmutableString(const ImmutableString& other) = delete;
			ImmutableString& operator=(const ImmutableString& other) = delete;

		private:
			WString mStringData;
		};
	}
}