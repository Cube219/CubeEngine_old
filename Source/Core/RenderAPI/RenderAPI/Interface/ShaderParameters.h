#pragma once

#include "../RenderAPIHeader.h"

#include "../Utilities/BaseRenderObject.h"
#include "ShaderParametersLayout.h"

namespace cube
{
	namespace render
	{
		class ShaderParameters : public BaseRenderObject
		{
		public:
			ShaderParameters(const char* debugName) : BaseRenderObject(debugName) {}
			virtual ~ShaderParameters() {}

			virtual void UpdateParameter(Uint32 bindIndex, void* pData, Uint32 size) = 0;
			virtual void UpdateParameter(Uint32 bindIndex, SPtr<TextureView>& textureView) = 0;
			virtual void UpdateParameter(Uint32 bindIndex, SPtr<Sampler>& sampler) = 0;
			virtual void UpdateParameter(Uint32 bindIndex, SPtr<TextureView>& textureView, SPtr<Sampler>& sampler) = 0;
		};
	} // namespace render
} // namespace cube
