#pragma once

#include "Base/BaseTypes.h"

namespace cube
{
	namespace platform
	{
		class DLib
		{
		public:
			DLib(){ }
			virtual ~DLib(){ }

			void* GetFunction(const String& name);

		protected:
			struct Data;
			Data* mData;
		};
	}
}
