#pragma once

#include "..\VulkanAPIHeader.h"

namespace cube
{
	namespace core
	{
		class VulkanInstance
		{
		public:
			VulkanInstance();
			~VulkanInstance();

			operator VkInstance() const
			{
				return mInstance;
			}

			void AddLayer(const char* layerName);
			void AddExtension(const char* extensionName);

			void SetApplicationName(const char* appName);
			void SetApplicationVersion(uint32_t appVersion);
			void SetEngineName(const char* engineName);
			void SetEngineVersion(uint32_t engineVersion);
			void SetAPIVersion(uint32_t apiVersion);

			void Create();

			const Vector<SPtr<VulkanPhysicalDevice>> EnumeratePhysicalDevices() const;

		private:
			VkInstance mInstance;

			const char* mApplicationName;
			uint32_t mApplicationVersion;
			const char* mEngineName;
			uint32_t mEngineVersion;
			uint32_t mApiVersion;

			Vector<const char*> mLayerNames;
			Vector<const char*> mExtensionNames;
		};

		// Define getters/setters
		inline void VulkanInstance::SetApplicationName(const char* appName)
		{
			mApplicationName = appName;
		}

		inline void VulkanInstance::SetApplicationVersion(uint32_t appVersion)
		{
			mApplicationVersion = appVersion;
		}

		inline void VulkanInstance::SetEngineName(const char* engineName)
		{
			mEngineName = engineName;
		}

		inline void VulkanInstance::SetEngineVersion(uint32_t engineVersion)
		{
			mEngineVersion = engineVersion;
		}

		inline void VulkanInstance::SetAPIVersion(uint32_t apiVersion)
		{
			mApiVersion = apiVersion;
		}
	}
}