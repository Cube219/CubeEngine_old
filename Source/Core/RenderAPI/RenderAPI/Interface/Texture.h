#pragma once

#include "../RenderAPIHeader.h"

#include "../Utilities/BaseRenderObject.h"
#include "RenderAPI/Interface/TextureView.h"

namespace cube
{
	namespace render
	{
		struct TextureData
		{
			const void* pData = nullptr;
			Uint64 size;
		};

		struct TextureAttribute : public BaseAttribute
		{
			ResourceUsage usage;
			TextureType type;
			TextureFormat format;
			TextureBindTypeFlags bindTypeFlags;

			TextureData textureData;
			Uint32 width;
			Uint32 height;
			Uint32 depth;
			Uint32 arraySize;
			
			Uint32 mipLevels;

			bool isDedicated = false;
		};

		class Texture : public BaseRenderObject
		{
		public:
			Texture(const char* debugName) : BaseRenderObject(debugName) {}
			virtual ~Texture() {}

			virtual void UpdateData(CommandList& cmdList, const void* pData, Uint64 size, Uint32 width, Uint32 height) = 0;

			virtual SPtr<TextureView> CreateView(const TextureViewAttribute& attr) = 0;

			TextureViewAttribute GetDefaultViewAttr() const { return mDefaultViewAttr; }

		protected:
			TextureViewAttribute mDefaultViewAttr;
		};
	} // namespace render
} // namespace cube
