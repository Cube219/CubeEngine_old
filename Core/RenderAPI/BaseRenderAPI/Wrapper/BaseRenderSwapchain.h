#pragma once

#include "..\BaseRenderAPIHeader.h"

namespace cube
{
	namespace core
	{
		class BaseRenderSwapchain
		{
		public:
			virtual ~BaseRenderSwapchain(){ }

			virtual uint32_t AcquireNextImageIndex(SPtr<BaseRenderSemaphore>& signalSemaphore) = 0;

			virtual void Recreate(uint32_t imageCount, uint32_t width, uint32_t height, bool vsync) = 0;

			virtual void Present(uint32_t waitSemaphoreNum, SPtr<BaseRenderSemaphore>* waitSemaphores) = 0;

		protected:
			BaseRenderSwapchain(){ }
		};
	}
}