#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		enum class CommandListUsage
		{
			Graphics,
			TransferImmediate,
			TransferDeferred,
			Compute
		};

		class CommandList
		{
		public:
			CommandList(CommandListUsage usage) : mUsage(usage)
			{}
			virtual ~CommandList() {}

			virtual void Begin() = 0;
			virtual void End() = 0;

			virtual void CopyBuffer(SPtr<BufferVk>& src, SPtr<BufferVk>& dst, Uint64 srcOffset, Uint64 dstOffset, Uint64 size) = 0;

			CommandListUsage GetUsage() const { return mUsage; }

		protected:
			CommandListUsage mUsage;
		};
	} // namespace render
} // namespace cube
