#pragma once

#include "../../EngineCoreHeader.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT Material
		{
		public:
			Material();
			~Material();

			void SetTexture(SPtr<Texture>& texture) { mTexture = texture; }

		private:
			SPtr<Texture> mTexture;
		};
	}
}