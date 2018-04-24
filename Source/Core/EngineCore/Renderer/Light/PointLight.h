#pragma once

#include "../../EngineCoreHeader.h"

#include "BaseLight.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT PointLight : public BaseLight
		{
		public:
			PointLight();
			virtual ~PointLight();

		private:
		};
	} // namespace core
} // namespace cube
