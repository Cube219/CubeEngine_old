#pragma once

#include "DebugStringHeap.h"

namespace cube
{
	namespace render
	{
		struct BaseAttribute
		{
			const char* debugName = "";
		};

		class BaseRenderObject
		{
		public:
			BaseRenderObject(const char* debugName) :
				mDebugName(debugName)
			{}
			virtual ~BaseRenderObject() {}

			const char* GetDebugName() const { return mDebugName; }

		protected:
			const char* mDebugName;
		};
	} // namespace render
} // namespace cube
