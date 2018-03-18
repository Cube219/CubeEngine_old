#pragma once

#include "Base/BaseTypes.h"

namespace cube
{
	namespace platform
	{
		class DLib
		{
		public:
			DLib();
			~DLib();

			void* GetFunction(const String& name);

		private:
			struct Data;
			Data* mData;
		};
	}
}
