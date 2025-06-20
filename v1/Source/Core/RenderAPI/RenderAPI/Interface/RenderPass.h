#pragma once

#include "../RenderAPIHeader.h"

#include "../Utilities/BaseRenderObject.h"

namespace cube
{
	namespace render
	{
		struct Subpass
		{
			Vector<Uint32> inputs;
			Vector<Uint32> colorOutputs;
			Uint32 depthStencilOutput;

			const char* debugName = nullptr;
		};

		struct SubpassDependency
		{
			Uint32 srcIndex;
			Uint32 dstIndex;
		};

		struct RenderPassAttribute : public BaseAttribute
		{
			Vector<SPtr<RenderTarget>> renderTargets;

			Vector<Subpass> subpasses;
			Vector<SubpassDependency> subpassdependencies;

			Uint32 width;
			Uint32 height;
		};

		class RenderPass : public BaseRenderObject
		{
		public:
			RenderPass(const RenderPassAttribute& attr) : BaseRenderObject(attr.debugName) {}
			virtual ~RenderPass() {}

			virtual void Resize(Uint32 width, Uint32 height) = 0;
		};
	} // namespace render
} // namespace cube
