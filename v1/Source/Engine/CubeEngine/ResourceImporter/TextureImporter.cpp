#include "TextureImporter.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "EngineCore/Renderer/Texture.h"
#include "EngineCore/Renderer/TextureData.h"

namespace cube
{
	Resource* TextureImporter::Import(SPtr<platform::File>& file, Json info)
	{
		uint64_t size = file->GetFileSize();

		void* rawData = malloc(size);
		uint64_t readSize = 0;
		file->Read(rawData, size, readSize);

		int width, height, channel;
		stbi_uc* imageData = stbi_load_from_memory(SCast(const stbi_uc*)(rawData), (int)(size), &width, &height, &channel, STBI_rgb_alpha);

		// Create texture resource
		SPtr<TextureData> textureData = GetTextureData(imageData, width, height, 1);
		Texture* texture = new Texture(textureData);
		
		stbi_image_free(imageData);
		free(rawData);

		return texture;
	}

	SPtr<TextureData> TextureImporter::GetTextureData(void* pData, Uint32 width, Uint32 height, Uint32 depth)
	{
		SPtr<TextureData> textureData = std::make_shared<TextureData>(width, height, depth);
		textureData->SetPixels(pData, width * height * depth);

		return textureData;
	}
} // namespace cube
