#pragma once

#include "../RenderAPIHeader.h"

#include "../Utilities/BaseRenderObject.h"

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
			// TODO: 많은 기능들 추가
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

			// TODO: DepthBoundTest는 나중에 구현?
			//       구현한다면 동적으로 구현
			//       VkPhysicalDeviceFeatures.depthBounds가 True여야 가능
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
			// TODO: LogicOp 구현?

			Vector<RenderTargetBlendState> renderTargets;
		};

		struct GraphicsPipelineStateAttribute : public BaseAttribute
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

			SPtr<RenderPass> renderPass;

			Vector<SPtr<ShaderParametersLayout>> shaderParameterLayouts;
		};

		class GraphicsPipelineState : public BaseRenderObject
		{
		public:
			GraphicsPipelineState(const char* debugName) : BaseRenderObject(debugName) {}
			virtual ~GraphicsPipelineState() {}
		};
	} // namespace render
} // namespace cube
