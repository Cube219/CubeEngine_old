#include "ComputePipelineStateVk.h"

#include "DeviceVk.h"
#include "../VulkanLogicalDevice.h"

namespace cube
{
	namespace render
	{
		ComputePipelineStateVk::ComputePipelineStateVk(DeviceVk& device, const ComputePipelineStateAttribute& attr)
		{
			VkComputePipelineCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.stage = ;
			info.layout = ;
			info.basePipelineHandle = ;
			info.basePipelineIndex = ;

			mPipeline = device.GetLogicalDevice()->CreateVkPipelineWrapper(info, VK_NULL_HANDLE, attr.debugName);
		}

		ComputePipelineStateVk::~ComputePipelineStateVk()
		{
			mPipeline.Release();
		}
	} // namespace render
} // namespace cube
