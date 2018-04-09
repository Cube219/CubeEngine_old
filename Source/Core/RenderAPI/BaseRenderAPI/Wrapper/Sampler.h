#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		class Sampler
		{
		public:
			virtual ~Sampler(){ }

		protected:
			Sampler(){ }
		};
	} // namespace render
} // namespace cube
