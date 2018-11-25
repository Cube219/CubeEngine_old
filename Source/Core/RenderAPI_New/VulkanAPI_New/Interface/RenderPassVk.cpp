#include "RenderPassVk.h"

#include "DeviceVk.h"
#include "RenderTargetVk.h"

namespace cube
{
	namespace render
	{
		RenderPassVk::RenderPassVk(DeviceVk& device, const RenderPassAttribute& attr)
		{
			// Attachment
			Vector<VkAttachmentDescription> attachmentDescs(attr.renderTargets.size());
			for(Uint64 i = 0; i < attachmentDescs.size(); i++) {
				attachmentDescs[i] = DPCast(RenderTargetVk)(attr.renderTargets[i])->GetVkAttachmentDescription();
			}

			// Subpass
			// Helper class for storaging attachment references
			struct SubpassDescHelper
			{
				VkSubpassDescription Get()
				{
					VkSubpassDescription desc;
					desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
					desc.flags = 0;
					desc.inputAttachmentCount = SCast(Uint32)(inputs.size());
					desc.pInputAttachments = inputs.data();

					desc.colorAttachmentCount = SCast(Uint32)(colors.size());
					desc.pColorAttachments = colors.data();
					// TODO: MultiSampling때 구현
					//       281p 책 참고
					desc.pResolveAttachments = nullptr;

					desc.pDepthStencilAttachment = &depthStencil;

					desc.preserveAttachmentCount = 0;
					desc.pPreserveAttachments = nullptr;

					return desc;
				}

				Vector<VkAttachmentReference> inputs;
				Vector<VkAttachmentReference> colors;
				VkAttachmentReference depthStencil;
			};
			Vector<SubpassDescHelper> subpassDescHelpers(attr.subpasses.size());
			Vector<VkSubpassDescription> subpassDescs(attr.subpasses.size());

			for(Uint64 i = 0; i < attr.subpasses.size(); i++) {
				auto& subpass = attr.subpasses[i];
				auto& descHelper = subpassDescHelpers[i];

				Uint32 index;

				descHelper.inputs.resize(subpass.inputs.size());
				for(Uint64 j = 0; j < descHelper.inputs.size(); j++) {
					index = subpass.inputs[j];
					descHelper.inputs[j] = { index, DPCast(RenderTargetVk)(attr.renderTargets[index])->GetVkImageLayout() };
				}

				descHelper.colors.resize(subpass.colorOutputs.size());
				for(Uint64 j = 0; j < descHelper.colors.size(); j++) {
					index = subpass.colorOutputs[j];
					descHelper.colors[j] = { index, DPCast(RenderTargetVk)(attr.renderTargets[index])->GetVkImageLayout() };
				}

				index = subpass.depthStencilOutput;
				descHelper.depthStencil = { index, DPCast(RenderTargetVk)(attr.renderTargets[index])->GetVkImageLayout() };

				subpassDescs[i] = descHelper.Get();
			}

			// Subpass dependencies
			Vector<VkSubpassDependency> subpassDependencies(attr.subpassdependencies.size());
			for(Uint64 i = 0; i < subpassDependencies.size(); i++) {
				auto& dependency = attr.subpassdependencies[i];

				subpassDependencies[i].srcSubpass = dependency.srcIndex;
				subpassDependencies[i].dstSubpass = dependency.dstIndex;
				subpassDependencies[i].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				subpassDependencies[i].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				subpassDependencies[i].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				subpassDependencies[i].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				subpassDependencies[i].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
				// TODO: 차후 커스텀 가능하게?
			}

			VkRenderPassCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.attachmentCount = SCast(Uint32)(attachmentDescs.size());
			info.pAttachments = attachmentDescs.data();
			info.subpassCount = SCast(Uint32)(subpassDescs.size());
			info.pSubpasses = subpassDescs.data();
			info.dependencyCount = SCast(Uint32)(subpassDependencies.size());
			info.pDependencies = subpassDependencies.data();

			mRenderPass = device.GetLogicalDevice()->CreateVkRenderPassWrapper(info, attr.debugName);
		}

		RenderPassVk::~RenderPassVk()
		{
			mRenderPass.Release();
		}
	} // namespace render
} // namespace cube
