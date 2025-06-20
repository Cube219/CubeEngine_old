#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		class Swapchain
		{
		public:
			virtual ~Swapchain(){ }

			virtual uint32_t AcquireNextImageIndex(SPtr<Semaphore>& signalSemaphore) = 0;

			virtual void Recreate(uint32_t imageCount, uint32_t width, uint32_t height, bool vsync) = 0;

			virtual void Present(uint32_t waitSemaphoreNum, SPtr<Semaphore>* waitSemaphores) = 0;

		protected:
			Swapchain(){ }
		};
	} // namespace render
} // namespace cube
