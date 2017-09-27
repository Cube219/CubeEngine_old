#pragma once

#include "..\EngineCoreHeader.h"

namespace cube
{
	namespace core
	{
		class PathString
		{
		public:
			friend class StringManager;

			PathString() = delete;
			~PathString();

			const WString& GetString() const
			{
				return mStringData;
			};

		private:
			PathString(WString& string);

			WString mStringData;
		};
	}
}