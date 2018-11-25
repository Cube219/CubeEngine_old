#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/RenderPass.h"

#include "../VkObject.h"

namespace cube
{
	namespace render
	{
		class RenderPassVk final : public RenderPass
		{
		public:
			RenderPassVk(DeviceVk& device, const RenderPassAttribute& attr);
			virtual ~RenderPassVk();

			VkRenderPass GetHandle() const { return mRenderPass.mObject; }
			const char* GetSubpassDebugName(Uint32 index) const { return mSubpassDebugNames[index]; }

		private:
			VkRenderPassWrapper mRenderPass;

			Vector<const char*> mSubpassDebugNames;
		};
	} // namespace render
} // namespace cube
