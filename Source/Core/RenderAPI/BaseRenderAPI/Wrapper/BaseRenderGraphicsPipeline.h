#pragma once

#include "../BaseRenderAPIHeader.h"

namespace cube
{
	namespace core
	{
		// TODO: ���߿� Compute�� ������
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

		struct BaseRenderGraphicsPipelineInitializer
		{
			struct VertexInputAttribute
			{
				uint32_t location;
				DataFormat format;
				uint32_t offset;
			};
			Vector<VertexInputAttribute> vertexInputAttributes;
			uint32_t vertexSize;

			VertexTopology vertexTopology;

			bool isViewportDynamic = false;
			Viewport viewport;
			bool isScissorDynamic = false;
			Rect2D scissor;

			struct Rasterizer
			{
				PolygonMode polygonMode;
				PolygonFrontFace polygonFrontFace;
				CullMode cullMode;
			};
			Rasterizer rasterizer;

			struct DepthStencil
			{
				bool depthTestEnable;
				bool depthBoundsTestEnable;
				bool depthWriteEnable;
				CompareOperator depthCompareOperator;

				bool stencilTestEnable;
				StencilOperatorState front;
				StencilOperatorState back;
				float minDepthBounds;
				float maxDepthBounds;
			};
			DepthStencil depthStencil;

			struct ColorBlendAttachment
			{
				bool blendEnable;
				BlendFactor srcColorBlendFactor;
				BlendFactor dstColorBlendFactor;
				BlendOperator colorBlendOp;
				BlendFactor srcAlphaBlendFactor;
				BlendFactor dstAlphaBlendFactor;
				BlendOperator alphaBlendOp;
			};
			Vector<ColorBlendAttachment> colorBlendAttachments;

			Vector<SPtr<BaseRenderShader>> shaders;
			
			Vector<SPtr<BaseRenderDescriptorSet>> descSets;

			SPtr<BaseRenderRenderPass> renderPass;
		};

		class BaseRenderGraphicsPipeline
		{
		public:
			virtual ~BaseRenderGraphicsPipeline(){ }

		protected:
			BaseRenderGraphicsPipeline(){ }
		};
	}
}