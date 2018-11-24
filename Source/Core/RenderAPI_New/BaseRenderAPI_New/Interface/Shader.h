#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct ShaderAttribute
		{
			ShaderType type;
			ShaderLanguage language;

			const char* code;
			const char* entryPoint;

			const char* debugName = nullptr;
		};

		class Shader
		{
		public:
			Shader() {}
			virtual ~Shader() {}

			ShaderType GetType() const { return mType; }

		protected:
			ShaderType mType;
		};
	} // namespace render
} // namespace cube
