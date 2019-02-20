#pragma once

#include "../RenderAPIHeader.h"

#include "../Utilities/BaseRenderObject.h"

namespace cube
{
	namespace render
	{
		struct SamplerAttribute : public BaseAttribute
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
		};

		class Sampler : public BaseRenderObject
		{
		public:
			Sampler(const char* debugName) : BaseRenderObject(debugName) {}
			virtual ~Sampler() {}
		};
	} // namespace render
} // namespace cube
