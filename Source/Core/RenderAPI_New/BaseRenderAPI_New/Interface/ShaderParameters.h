#pragma once

#include "../RenderAPIHeader.h"

#include "ShaderParametersLayout.h"

namespace cube
{
	namespace render
	{
		class ShaderParameters
		{
		public:
			ShaderParameters() {}
			virtual ~ShaderParameters() {}

			virtual void UpdateParameter(Uint32 bindIndex, void* pData, Uint32 size) = 0;
		};
	} // namespace render
} // namespace cube
