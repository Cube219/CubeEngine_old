#pragma once

#include "../RenderAPIHeader.h"

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

		struct RenderPassAttribute
		{
			Vector<SPtr<RenderTarget>> renderTargets;

			Vector<Subpass> subpasses;
			Vector<SubpassDependency> subpassdependencies;

			const char* debugName = nullptr;
		};

		class RenderPass
		{
		public:
			RenderPass() {}
			virtual ~RenderPass() {}
		};
	} // namespace render
} // namespace cube
