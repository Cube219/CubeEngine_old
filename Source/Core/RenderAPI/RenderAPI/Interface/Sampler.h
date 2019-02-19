#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct SamplerAttribute
		{
			FilterType minFilter;
			FilterType magFilter;
			FilterType mipMapFilter;

			AddressMode addressU;
			AddressMode addressV;
			AddressMode addressW;

			Uint32 maxAnisotropy;

			float mipLodBias;
			float minLod;
			float maxLod;

			float borderColor[4];

			const char* debugName = nullptr;
		};

		class Sampler
		{
		public:
			Sampler() {}
			virtual ~Sampler() {}
		};
	} // namespace render
} // namespace cube
