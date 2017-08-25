#pragma once

#include "..\BaseRenderAPIHeader.h"

namespace cube
{
	namespace core
	{
		// TODO: 나중에 Compute랑 나누기
		enum class PipelineStageBits
		{
			TopBit = 0x00000001,
			DrawIndirectBit = 0x00000002,
			VertexInputBit = 0x00000004,
			VertexShaderBit = 0x00000008,
			TessellationControlShaderBit = 0x00000010,
			TessellationEvaluationShaderBit = 0x00000020,
			GeometryShaderBit = 0x00000040,
			FragmentShaderBit = 0x00000080,
			EarlyFragmentShaderBit = 0x00000100,
			LateFragmentShaderBit = 0x00000200,
			ColorAttachmentOutputBit = 0x00000400,
			ComputeShaderBit = 0x00000800,
			TransferBit = 0x00001000,
			BottomBit = 0x00002000,
			HostBit = 0x00004000,
			AllGraphicsBit = 0x00008000,
			AllCommandsBit = 0x00010000
		};
		SET_ENUM_AS_FLAGS(PipelineStageBits)

		class BaseRenderGraphicsPipeline
		{
		public:
			virtual ~BaseRenderGraphicsPipeline(){ }

			// TODO: binding으로 여러 vertex정보를 추가...? (지금은 0번만 씀)
			virtual void AddVertexInputAttribute(uint32_t location, DataFormat format, uint32_t offset) = 0;
			virtual void SetVertexInput(uint32_t sizePerVertex) = 0;

			virtual void SetVertexTopology(VertexTopology topology) = 0;

			virtual void AddViewport(Viewport viewport, bool isDynamic) = 0;
			virtual void AddScissor(Rect2D scissor, bool isDynamic) = 0;
			virtual void SetViewportState() = 0;

			virtual void SetRasterizer(PolygonMode polygonMode, PolygonFrontFace polygonFrontFace, CullMode cullMode) = 0;

			// TODO: Multisample구현시 구현
			virtual void SetMultisampler() = 0;

			virtual void SetDepthStencil(bool depthTestEnable, bool depthBoundsTestEnable, bool depthWriteEnable, CompareOperator depthCompareOperator,
				bool stencilTestEnable, StencilOperatorState front, StencilOperatorState back, float minDepthBounds, float maxDepthBounds) = 0;

			virtual void AddColorBlendAttachment(bool blendEnable,
				BlendFactor srcColorBlendFactor, BlendFactor dstColorBlendFactor, BlendOperator colorBlendOp,
				BlendFactor srcAlphaBlendFactor, BlendFactor dstAlphaBlendFactor, BlendOperator alphaBlendOp) = 0;
			virtual void SetColorBlend(bool logicOpEnable, LogicOperator logicOp,
				float blendConstant1, float blendConstant2, float blendConstant3, float blendConstant4) = 0;

			virtual void AddShader(SPtr<BaseRenderShader>& shader) = 0;

			virtual void AddDescriptorSet(SPtr<BaseRenderDescriptorSet>& descSet) = 0;

			virtual void Create(SPtr<BaseRenderRenderPass>& renderPass) = 0;

		protected:
			BaseRenderGraphicsPipeline(){ }

			//std::vector<SPtr<BaseRenderShader>> mShaders;
			//std::vector<SPtr<BaseRenderDescriptorSet>> mDescriptorSets;
		};
	}
}