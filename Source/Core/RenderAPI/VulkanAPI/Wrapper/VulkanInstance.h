#pragma once

#include "../VulkanAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct VulkanInstanceInitializer
		{
			Vector<const char*> layerNames;
			Vector<const char*> extensionNames;

			const char* appName;
			uint32_t appVersion;
			const char* engineName;
			uint32_t engineVersion;
			uint32_t apiVersion;
		};

		class VulkanInstance
		{
		public:
			VulkanInstance(VulkanInstanceInitializer& initializer);
			~VulkanInstance();

			VkInstance GetHandle() const { return mInstance; }

			const Vector<SPtr<VulkanPhysicalDevice>> EnumeratePhysicalDevices() const;

		private:
			VkInstance mInstance;
		};
	} // namespace render
} // namespace cube
