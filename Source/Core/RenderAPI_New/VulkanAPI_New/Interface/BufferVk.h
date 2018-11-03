#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/Buffer.h"

#include "../VulkanMemoryManager.h"

namespace cube
{
	namespace render
	{
		class BufferVk final : public Buffer
		{
		public:
			BufferVk(SPtr<DeviceVk> device, const BufferAttribute& attr);
			virtual ~BufferVk();

			VkBuffer GetHandle() const { return mBuffer; }

			virtual void UpdateData(Uint64 offset, Uint64 size, const void* pData) override final;
			virtual void CopyData(const SPtr<Buffer>& src, Uint64 srcOffset, Uint64 dstOffset, Uint64 size) override final;

			virtual void Map(void*& pMappedData) override final;
			virtual void Unmap() override final;

			virtual void CreateView() override final;

		private:
			SPtr<DeviceVk> mDevice;
			VkBuffer mBuffer;

			VulkanAllocation mMemoryAllocation;
		};
	} // namespace render
} // namespace cube
