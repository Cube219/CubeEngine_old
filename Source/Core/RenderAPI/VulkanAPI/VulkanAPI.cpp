#include "VulkanAPI.h"

#include "Wrapper/VulkanInstance.h"
#include "Wrapper/VulkanPhysicalDevice.h"
#include "Wrapper/VulkanDevice.h"
#include "Wrapper/VulkanCommandBuffer.h"
#include "Wrapper/VulkanQueue.h"
#include "Wrapper/VulkanWindowSurface.h"
#include "Wrapper/VulkanSwapchain.h"
#include "Wrapper/VulkanImage.h"
#include "Wrapper/VulkanSampler.h"
#include "Wrapper/VulkanDescriptor.h"
#include "Wrapper/VulkanRenderPass.h"
#include "Wrapper/VulkanFramebuffer.h"
#include "Wrapper/VulkanShader.h"
#include "Wrapper/VulkanGraphicsPipeline.h"
#include "Wrapper/VulkanBuffer.h"
#include "Wrapper/VulkanRenderPass.h"
#include "Wrapper/VulkanSwapchain.h"
#include "Wrapper/VulkanFence.h"
#include "Wrapper/VulkanSemaphore.h"

#include "WinPlatform/WinPlatform.h"

namespace cube
{
	namespace core
	{
		BaseRenderAPI* CreateAPI()
		{
			return new VulkanAPI();
		}

		VulkanAPI::VulkanAPI()
		{
		}

		VulkanAPI::~VulkanAPI()
		{
		}

		void VulkanAPI::Init(SPtr<platform::BasePlatform>& platform)
		{
			// Init instance
			VulkanInstanceInitializer instanceInit;
			instanceInit.appName = "CubeEngine Application";
			instanceInit.appVersion = 1;
			instanceInit.engineName = "CubeEngine";
			instanceInit.engineVersion = 1;
			instanceInit.apiVersion = VK_API_VERSION_1_0;
#ifdef _DEBUG
			instanceInit.layerNames.push_back("VK_LAYER_LUNARG_standard_validation");
#endif // _DEBUG
			instanceInit.extensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef _WIN32
			instanceInit.extensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif // _WIN32
			mInstance = std::make_shared<VulkanInstance>(instanceInit);

			// Enumerate physical devices
			mPhysicalDevices = mInstance->EnumeratePhysicalDevices();
			mMainPhysicalDevice = mPhysicalDevices[0];

			// Create a device
			VulkanDeviceInitializer deviceInitializer;
			deviceInitializer.AddExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

			deviceInitializer.AddFeatures(VulkanPhysicalDeviceFeature::TessellationShader, true, true);
			deviceInitializer.AddFeatures(VulkanPhysicalDeviceFeature::GeometryShader, true, true);
			deviceInitializer.AddFeatures(VulkanPhysicalDeviceFeature::SamplerAnisotropy, true, true);
			deviceInitializer.AddFeatures(VulkanPhysicalDeviceFeature::MultiDrawIndirect, true, false);

			mDevice = std::make_shared<VulkanDevice>(mMainPhysicalDevice, deviceInitializer);


			// Create a command pool
			mCommandPool = std::make_shared<VulkanCommandPool>(mDevice, mDevice->GetGraphicsQueueFamily());

			// Create a surface
#ifdef _WIN32
			SPtr<platform::WinPlatform> win32Platform = DPCast(platform::WinPlatform)(platform);
			mWindowSurface = std::make_shared<VulkanWindowSurface>(mInstance, mMainPhysicalDevice, mDevice, win32Platform->GetInstance(), win32Platform->GetWindow());
#endif // _WIN32

			// Create a descriptor pool
			mDescriptorPool = std::make_shared<VulkanDescriptorPool>(mDevice);
		}

		SPtr<BaseRenderBuffer> VulkanAPI::CreateBuffer(uint64_t size, BufferTypeBits types)
		{
			VkBufferUsageFlags usageFlags = 0;
			if(SCast(int)(types | BufferTypeBits::Uniform) > 0)
				usageFlags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			if(SCast(int)(types | BufferTypeBits::Vertex) > 0)
				usageFlags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			if(SCast(int)(types | BufferTypeBits::Index) > 0)
				usageFlags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			if(SCast(int)(types | BufferTypeBits::TransferSource) > 0)
				usageFlags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			return std::make_shared<VulkanBuffer>(mDevice, usageFlags, size, nullptr, VK_SHARING_MODE_EXCLUSIVE);
		}

		SPtr<BaseRenderDescriptorSet> VulkanAPI::CreateDescriptorSet(BaseRenderDescriptorSetInitializer& initializer)
		{
			return std::make_shared<VulkanDescriptorSet>(mDevice, mDescriptorPool, initializer);
		}

		SPtr<BaseRenderQueue> VulkanAPI::GetQueue(QueueTypeBits types, uint32_t index)
		{
			return mDevice->GetQueue(GetVkQueueFlags(types), index);
		}

		SPtr<BaseRenderSwapchain> VulkanAPI::CreateSwapchain()
		{
			return std::make_shared<VulkanSwapchain>(mDevice, mWindowSurface, 2, 10, 10, false);
		}

		SPtr<BaseRenderRenderPass> VulkanAPI::CreateRenderPass(BaseRenderRenderPassInitializer& initializer)
		{
			return std::make_shared<VulkanRenderPass>(mDevice, initializer);
		}

		SPtr<BaseRenderShader> VulkanAPI::CreateShader(BaseRenderShaderInitializer& initializer)
		{
			return std::make_shared<VulkanShader>(mDevice, initializer);
		}

		SPtr<BaseRenderGraphicsPipeline> VulkanAPI::CreateGraphicsPipeline(BaseRenderGraphicsPipelineInitializer& initializer)
		{
			return std::make_shared<VulkanGraphicsPipeline>(mDevice, initializer);
		}

		SPtr<BaseRenderCommandBuffer> VulkanAPI::CreateCommandBuffer()
		{
			return mCommandPool->AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		}

		SPtr<BaseRenderImage> VulkanAPI::CreateImage(BaseRenderImageInitializer& initializer)
		{
			return std::make_shared<VulkanImage>(mDevice, initializer);
		}
		SPtr<BaseRenderSampler> VulkanAPI::CreateSampler()
		{
			return std::make_shared<VulkanSampler>(mDevice);
		}

		SPtr<BaseRenderFence> VulkanAPI::CreateFence()
		{
			return std::make_shared<VulkanFence>(mDevice);
		}

		SPtr<BaseRenderSemaphore> VulkanAPI::CreateSemaphore()
		{
			return std::make_shared<VulkanSemaphore>(mDevice);
		}
	}
}