#pragma once

#include "../EngineCoreHeader.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT RenderObject
		{
		public:
			RenderObject(){ }
			virtual ~RenderObject(){ }
		};

		class ENGINE_CORE_EXPORT RenderObject_RT
		{
		public:
			RenderObject_RT(){ }
			virtual ~RenderObject_RT(){ }
		};
	} // namespace core
} // namespace cube
