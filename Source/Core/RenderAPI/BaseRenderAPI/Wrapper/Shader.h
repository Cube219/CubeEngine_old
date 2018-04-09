#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct ShaderInitializer
		{
			ShaderLanguage language;
			ShaderTypeBits type;
			const char* code;
			const char* entryPoint;
		};

		class Shader
		{
		public:
			virtual ~Shader(){ }

		protected:
			Shader(){ }
		};
	} // namespace render
} // namespace cube
