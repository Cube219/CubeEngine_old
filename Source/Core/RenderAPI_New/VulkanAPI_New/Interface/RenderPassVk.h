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

			virtual void Resize(Uint32 width, Uint32 height) override final;

			VkRenderPass GetHandle() const { return mRenderPass.mObject; }

			const char* GetSubpassDebugName(Uint32 index) const { return mSubpassDebugNames[index]; }

			Uint32 GetRenderTargetNum() const { return SCast(Uint32)(mRenderTargets.size()); }
			const Vector<SPtr<RenderTargetVk>>& GetRenderTargets() const { return mRenderTargets; }

			VkFramebuffer GetVkFramebuffer() const { return mFramebuffers[0].mObject; }
			Rect2D GetFramebufferRect2D() const { return mFramebufferRect2D; }

		private:
			void CreateFramebuffer(Uint32 width, Uint32 height);

			VkRenderPassWrapper mRenderPass;

			DeviceVk& mDevice;

			Vector<const char*> mSubpassDebugNames;

			Vector<SPtr<RenderTargetVk>> mRenderTargets;
			bool mHasSwapchainRenderTarget;
			Vector<VkFramebufferWrapper> mFramebuffers;
			Rect2D mFramebufferRect2D;
		};
	} // namespace render
} // namespace cube
