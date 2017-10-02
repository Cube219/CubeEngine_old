#include "VulkanAPI.h"

#include "Wrapper/VulkanInstance.h"
#include "Wrapper/VulkanPhysicalDevice.h"
#include "Wrapper/VulkanDevice.h"
#include "Wrapper/VulkanCommandBuffer.h"
#include "Wrapper/VulkanQueue.h"
#include "Wrapper/VulkanWindowSurface.h"
#include "Wrapper/VulkanSwapchain.h"
#include "Wrapper/VulkanImage.h"
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

		// TODO: 나중에 주석 정리 (좀 지우기)
		void VulkanAPI::Init(SPtr<platform::BasePlatform>& platform)
		{
			// Init instance
			mInstance = std::make_shared<VulkanInstance>();
			mInstance->SetApplicationName("CubeEngine Application");
			mInstance->SetApplicationVersion(1);
			mInstance->SetEngineName("CubeEngine");
			mInstance->SetEngineVersion(1);
			mInstance->SetAPIVersion(VK_API_VERSION_1_0);
#ifdef _DEBUG
			mInstance->AddLayer("VK_LAYER_LUNARG_standard_validation");
#endif // _DEBUG
			mInstance->AddExtension(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef _WIN32
			mInstance->AddExtension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif // _WIN32
			mInstance->Create();

			// Enumerate physical devices
			mPhysicalDevices = mInstance->EnumeratePhysicalDevices();
			mMainPhysicalDevice = mPhysicalDevices[0];

			// Create a device
			mDevice = std::make_shared<VulkanDevice>();

			mDevice->AddExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

			// TODO: 일단 GraphicsQueue만...
			auto graphicsQueueFamily = mMainPhysicalDevice->GetQueueFamily(VK_QUEUE_GRAPHICS_BIT);
			mDevice->CreateDeviceQueue(graphicsQueueFamily, graphicsQueueFamily.mProperties.queueCount);

			mDevice->SetFeatures(VulkanPhysicalDeviceFeature::TessellationShader, true, true);
			mDevice->SetFeatures(VulkanPhysicalDeviceFeature::GeometryShader, true, true);
			mDevice->SetFeatures(VulkanPhysicalDeviceFeature::MultiDrawIndirect, true, false);

			mDevice->Create(mMainPhysicalDevice);

			// Create a command pool
			mCommandPool = std::make_shared<VulkanCommandPool>(mDevice, mDevice->GetGraphicsQueueFamily());

			// Create a surface
#ifdef _WIN32
			SPtr<platform::WinPlatform> win32Platform = DPCast(platform::WinPlatform)(platform);
			mWindowSurface = std::make_shared<VulkanWindowSurface>(mInstance, mMainPhysicalDevice, win32Platform->GetInstance(), win32Platform->GetWindow());
#endif // _WIN32

			// Create a descriptor pool
			mDescriptorPool = std::make_shared<VulkanDescriptorPool>();
			mDescriptorPool->AddDescriptorTypeToAllocate(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10);
			mDescriptorPool->Create(mDevice);
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

		SPtr<BaseRenderDescriptorSet> VulkanAPI::CreateDescriptorSet()
		{
			return std::make_shared<VulkanDescriptorSet>(mDevice, mDescriptorPool);
		}

		SPtr<BaseRenderQueue> VulkanAPI::GetQueue(QueueTypeBits types, uint32_t index)
		{
			return mDevice->GetQueue(GetVkQueueFlags(types), index);
		}

		SPtr<BaseRenderSwapchain> VulkanAPI::CreateSwapchain()
		{
			return std::make_shared<VulkanSwapchain>(mDevice, mWindowSurface, 2, 10, 10, false);
		}

		SPtr<BaseRenderRenderPass> VulkanAPI::CreateRenderPass()
		{
			return std::make_shared<VulkanRenderPass>(mDevice);
		}

		SPtr<BaseRenderShader> VulkanAPI::CreateShader(ShaderType type, String& code, String& entryPoint)
		{
			VkShaderStageFlagBits shaderStageFlag;

			switch(type) {
				case ShaderType::GLSL_Vertex:
					shaderStageFlag = VK_SHADER_STAGE_VERTEX_BIT;
					break;
				case ShaderType::GLSL_Fragment:
					shaderStageFlag = VK_SHADER_STAGE_FRAGMENT_BIT;
					break;

				default:
					PrintlnLogWithSayer(L"VulkanAPI", L"Unknown shader type");
					break;
			}

			return std::make_shared<VulkanShader>(mDevice, shaderStageFlag, code.c_str(), entryPoint.c_str());
		}

		SPtr<BaseRenderGraphicsPipeline> VulkanAPI::CreateGraphicsPipeline()
		{
			return std::make_shared<VulkanGraphicsPipeline>(mDevice);
		}

		SPtr<BaseRenderCommandBuffer> VulkanAPI::CreateCommandBuffer()
		{
			return mCommandPool->AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		}

		SPtr<BaseRenderImage> VulkanAPI::CreateImage(ImageType type, DataFormat format,
			uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, ImageUsageBits usage, bool optimal)
		{
			VkExtent3D extent = {width, height, depth};

			return std::make_shared<VulkanImage>(mDevice,
				GetVkImageType(type), GetVkFormat(format), extent, mipLevels, 1,
				VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_LAYOUT_UNDEFINED, GetVkImageUsageFlags(usage), VK_SHARING_MODE_EXCLUSIVE, optimal);
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