#pragma once
#pragma warning(disable:4251)
#pragma warning(disable:4275)

#include "Base/BaseTypes.h"

#include <vulkan/vulkan.h>

namespace cube
{
	namespace render
	{
		class VulkanPhysicalDevice;

		class DeviceVk;
	} // namespace render
} // namespace cube

#ifdef VULKAN_API_EXPORTS
#define VULKAN_API_EXPORT __declspec(dllexport) 
#else // VULKAN_API_EXPORTS
#define VULKAN_API_EXPORT __declspec(dllimport) 
#endif // VULKAN_API_EXPORTS
