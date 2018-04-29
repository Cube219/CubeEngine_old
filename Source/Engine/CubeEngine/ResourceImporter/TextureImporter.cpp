#include "TextureImporter.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "EngineCore/Renderer/Texture.h"

namespace cube
{
	core::Resource* TextureImporter::Import(SPtr<platform::File>& file, Json info)
	{
		uint64_t size = file->GetFileSize();

		void* rawData = malloc(size);
		uint64_t readSize = 0;
		file->Read(rawData, size, readSize);

		int width, height, channel;
		stbi_uc* imageData = stbi_load_from_memory(SCast(const stbi_uc*)(rawData), (int)(size), &width, &height, &channel, STBI_rgb_alpha);

		// Create texture resource
		core::Texture* texture = new core::Texture();
		texture->WriteData(mRenderAPI, imageData, width * height * 4, width, height);

		stbi_image_free(imageData);
		free(rawData);

		return texture;
	}
} // namespace cube
