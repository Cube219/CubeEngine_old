#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct TextureData
		{
			const void* pData = nullptr;
			Uint64 size;
		};

		struct TextureAttribute
		{
			ResourceUsage usage;
			TextureType type;
			TextureFormat format;
			TextureBindTypeFlags bindTypeFlags = 0;

			TextureData textureData;
			Uint32 width;
			Uint32 height;
			Uint32 depth;
			
			Uint32 mipLevels;

			const char* debugName = nullptr;
		};

		class Texture
		{
		public:
			Texture() {}
			virtual ~Texture() {}

			virtual void UpdateData(CommandList& cmdList, const void* pData, Uint64 size, Uint32 width, Uint32 height) = 0;
		};
	} // namespace render
} // namespace cube
