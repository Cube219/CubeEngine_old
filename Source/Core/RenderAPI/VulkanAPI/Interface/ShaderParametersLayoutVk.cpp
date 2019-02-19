#include "ShaderParametersLayoutVk.h"

#include "RenderAPI/Interface/ShaderParameters.h"

#include "DeviceVk.h"
#include "ShaderParametersVk.h"
#include "../VulkanLogicalDevice.h"
#include "../VulkanTypeConversion.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		ShaderParametersLayoutVk::ShaderParametersLayoutVk(DeviceVk& device, const ShaderParametersLayoutAttribute& attr) :
			ShaderParametersLayout(attr.debugName),
			mShaderParameterManager(device.GetShaderParameterManager()),
			mParameterList(attr.paramInfos)
		{
			auto logical = device.GetLogicalDevice();
			mDevice = logical->GetHandle();

			VkDescriptorSetLayoutCreateInfo layoutInfo;
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.pNext = nullptr;
			layoutInfo.flags = 0;

			Uint32 maxBindingNum = 0;
			for(auto& param : attr.paramInfos) {
				if(param.bindIndex > maxBindingNum)
					maxBindingNum = param.bindIndex;
			}

			Vector<VkDescriptorSetLayoutBinding> bindings;
			for(auto& param : attr.paramInfos) {
				VkDescriptorSetLayoutBinding binding;

				switch(param.type) {
					case ShaderParameterType::RawData:
						mHasRawDataParameter = true;
						mRawDataParameterSize = param.size;
						continue; // RawData(Push constant) doesn't bind descriptor set

					case ShaderParameterType::ConstantBuffer:
						binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
						break;

					case ShaderParameterType::StorageBuffer:
						binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
						break;

					case ShaderParameterType::ConstantImage:
						binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
						break;

					case ShaderParameterType::StorageImage:
						binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
						break;

					case ShaderParameterType::SampledImage:
						binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						break;

					case ShaderParameterType::Sampler:
						binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
						break;

					default:
						ASSERTION_FAILED("Unknown ShaderParameterType ({0})", (int)param.type);
				}

				binding.binding = param.bindIndex;
				binding.descriptorCount = param.count;
				binding.stageFlags = VK_SHADER_STAGE_ALL;
				binding.pImmutableSamplers = nullptr;

				bindings.push_back(binding);
			}

			layoutInfo.bindingCount = SCast(Uint32)(bindings.size());
			layoutInfo.pBindings = bindings.data();

			mDescriptorSetLayout = logical->CreateVkDescriptorSetLayoutWrapper(layoutInfo, attr.debugName);
		}

		ShaderParametersLayoutVk::~ShaderParametersLayoutVk()
		{
		}

		SPtr<ShaderParameters> ShaderParametersLayoutVk::CreateParameters()
		{
			return std::make_shared<ShaderParametersVk>(mDevice, mShaderParameterManager, *this, mDebugName);
		}
	} // namespace render
} // namespace cube
