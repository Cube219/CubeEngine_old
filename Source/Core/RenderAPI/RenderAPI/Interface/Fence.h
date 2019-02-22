#pragma once

#include "../RenderAPIHeader.h"

#include "../Utilities/BaseRenderObject.h"

namespace cube
{
	namespace render
	{
		enum class FenceWaitResult
		{
			Success,
			Timeout,
			Error
		};

		class Fence : public BaseRenderObject
		{
		public:
			Fence(const char* debugName) : BaseRenderObject(debugName) {}
			virtual ~Fence() {}

			virtual void Release() = 0;

			virtual FenceWaitResult Wait(float timeout) = 0;

			bool IsFinished() { return Wait(0.0f) == FenceWaitResult::Success; }
		};
	} // namespace render
} // namespace cube
