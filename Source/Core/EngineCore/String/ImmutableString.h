#pragma once

#include "../EngineCoreHeader.h"

namespace cube
{
	class ENGINE_CORE_EXPORT ImmutableString
	{
	public:
		ImmutableString();
		~ImmutableString();

		ImmutableString(const ImmutableString& other) = delete;
		ImmutableString& operator=(const ImmutableString& other) = delete;

	private:
	};
} // namespace cube
