#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct InputLayout
		{
			Uint32 size;

			struct Element
			{
				Uint32 offset;
				TextureFormat format;
			};
			Vector<Element> elements;
		};

		struct RasterizerState
		{
			FillMode fillMode;
			CullMode cullMode;
			TriangleFrontFace frontFace;
			// TODO: ���� ��ɵ� �߰�
		};

		struct StencilState
		{
			StencilOperator depthFailOp;
			StencilOperator failOp;
			StencilOperator passOp;
			ComparisonFunction compareFunc;
			Uint8 readMask;
			Uint8 writeMask;
		};
		struct DepthStencilState
		{
			bool enableDepthTest;
			bool enableDepthWrite;
			ComparisonFunction depthCompareFunc;
			// depthBoundsTestEnable

			bool enableStencilTest;
			StencilState frontFace;
			StencilState backFace;

			// TODO: DepthBoundTest�� ���߿� ����?
			//       �����Ѵٸ� �������� ����
			//       VkPhysicalDeviceFeatures.depthBounds�� True���� ����
		};

		struct RenderTargetBlendState
		{
			bool enableBlend;

			BlendFactor srcBlend;
			BlendFactor dstBlend;
			BlendOperator blendOp;

			BlendFactor srcAlphaBlend;
			BlendFactor dstAlphaBlend;
			BlendOperator alphaBlendOp;

			ColorWriteMaskFlags writeMask;
		};
		struct BlendState
		{
			// TODO: LogicOp ����?

			Vector<RenderTargetBlendState> renderTargets;
		};

		struct GraphicsPipelineStateAttribute
		{
			Vector<InputLayout> inputLayouts;

			PrimitiveTopology primitiveTopology;

			RasterizerState rasterizerState;

			DepthStencilState depthStencilState;

			BlendState blendState;

			Vector<TextureFormat> renderTargetFormats;
			TextureFormat depthStencilFormat;

			SPtr<Shader> vertexShader = nullptr;
			SPtr<Shader> pixelShader = nullptr;
			SPtr<Shader> domainShader = nullptr;
			SPtr<Shader> hullShader = nullptr;
			SPtr<Shader> geometryShader = nullptr;
			// RN: PipelineLayout�� ��� �ұ�? RenderPass��?

			const char* debugName = nullptr;
		};

		class GraphicsPipelineState
		{
		public:
			GraphicsPipelineState() {}
			virtual ~GraphicsPipelineState() {}
		};
	} // namespace render
} // namespace cube
