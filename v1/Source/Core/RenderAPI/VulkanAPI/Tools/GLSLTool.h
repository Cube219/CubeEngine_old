#pragma once

#include "Base/BaseTypes.h"

#include "RenderAPI/Interface/RenderTypes.h"

struct TBuiltInResource;

namespace cube
{
	namespace render
	{
		class GLSLTool
		{
		public:
			GLSLTool() = delete;
			~GLSLTool() = delete;

			static void Init();
			static void Finalize();

			static Vector<Uint32> ToSPIRV(ShaderType type, const char* code);

		private:
			static TBuiltInResource InitResources();
		};
	} // namespace render
} // namespace cube
