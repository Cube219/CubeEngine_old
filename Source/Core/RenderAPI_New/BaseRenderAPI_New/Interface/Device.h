#pragma once

#include "../RenderAPIHeader.h"

#include "CommandList.h"

namespace cube
{
	namespace render
	{
		struct DeviceAttribute
		{
			uint32_t GPUIndex;
			bool enableDebugLayer;
			const char* debugName;
		};

		class Device
		{
		public:
			Device() {}
			virtual ~Device() {}

			virtual SPtr<Buffer> CreateBuffer(const BufferAttribute& attr) = 0;
			virtual SPtr<CommandList> GetCommandList(CommandListUsage usage) = 0;

			virtual SPtr<Fence> SubmitCommandList(SPtr<CommandList>& commandList) = 0;

			virtual void FinishFrame() = 0;
		};
	} // namespace render
} // namespace cube
