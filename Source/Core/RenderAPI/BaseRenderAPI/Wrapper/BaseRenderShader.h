#pragma once

#include "../BaseRenderAPIHeader.h"

namespace cube
{
	namespace core
	{
		struct BaseRenderShaderInitializer
		{
			ShaderType type;
			const char* code;
			const char* entryPoint;
		};

		class BaseRenderShader
		{
		public:
			virtual ~BaseRenderShader(){ }

		protected:
			BaseRenderShader(){ }
		};
	}
}