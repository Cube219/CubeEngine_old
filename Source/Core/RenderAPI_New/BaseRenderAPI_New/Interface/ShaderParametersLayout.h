#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct ShaderParameterInfo
		{
			ShaderParameterType type;
			Uint32 size;
			Uint32 count;
			Uint32 bindIndex;
			bool isChangedPerFrame;

			const char* debugName = "";
		};

		struct ShaderParametersLayoutAttribute
		{
			Vector<ShaderParameterInfo> paramInfos;

			const char* debugName = "";
		};

		class ShaderParametersLayout
		{
		public:
			ShaderParametersLayout() {}
			virtual ~ShaderParametersLayout() {}

			virtual SPtr<ShaderParameters> GetParameters() = 0;
		};
	} // namespace render
} // namespace cube
