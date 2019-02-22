#include "ComputePipelineStateVk.h"

#include "DeviceVk.h"
#include "ShaderVk.h"
#include "ShaderParametersLayoutVk.h"
#include "../VulkanLogicalDevice.h"

namespace cube
{
	namespace render
	{
		ComputePipelineStateVk::ComputePipelineStateVk(DeviceVk& device, const ComputePipelineStateAttribute& attr) :
			ComputePipelineState(attr.debugName)
		{
			// Shader stage
			VkPipelineShaderStageCreateInfo shaderStage;
			shaderStage = DPCast(ShaderVk)(attr.shader)->GetPipelineShaderStageInfo();

			// Pipeline layout (ShaderParametersLayout)
			// Equivalent in GraphicsPipelineStateVk
			VkPipelineLayoutCreateInfo layoutInfo;
			layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			layoutInfo.pNext = nullptr;
			layoutInfo.flags = 0;

			Vector<VkDescriptorSetLayout> descSetLayouts(attr.shaderParameterLayouts.size());

			VkPushConstantRange pushConstantRange;
			layoutInfo.pushConstantRangeCount = 0;
			layoutInfo.pPushConstantRanges = nullptr;

			for(Uint64 i = 0; i < attr.shaderParameterLayouts.size(); i++) {
				SPtr<ShaderParametersLayoutVk> layout = DPCast(ShaderParametersLayoutVk)(attr.shaderParameterLayouts[i]);

				if(layout->HasRawDataParameter() == true) {
					CHECK(layoutInfo.pushConstantRangeCount == 0, "Pipeline doen't have two or more RawDataParameter.");

					pushConstantRange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT; // Different in GraphicsPipelineStateVk
					pushConstantRange.offset = 0;
					pushConstantRange.size = layout->GetRawDataParameterSize();

					layoutInfo.pushConstantRangeCount = 1;
					layoutInfo.pPushConstantRanges = &pushConstantRange;
				}

				descSetLayouts[i] = layout->GetVkDescriptorSetLayout();
			}

			layoutInfo.setLayoutCount = SCast(Uint32)(descSetLayouts.size());
			layoutInfo.pSetLayouts = descSetLayouts.data();
			
			mLayout = device.GetLogicalDevice()->CreateVkPipelineLayoutWrapper(layoutInfo,
				fmt::format("PipelineLayout for '{0}'", attr.debugName).c_str());

			VkComputePipelineCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.stage = shaderStage;
			info.layout = mLayout.mObject;
			info.basePipelineHandle = VK_NULL_HANDLE;
			info.basePipelineIndex = 0;

			mPipeline = device.GetLogicalDevice()->CreateVkPipelineWrapper(info, VK_NULL_HANDLE, attr.debugName);
		}

		ComputePipelineStateVk::~ComputePipelineStateVk()
		{
			mLayout.Release();
			mPipeline.Release();
		}
	} // namespace render
} // namespace cube
