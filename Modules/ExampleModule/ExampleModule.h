#pragma once

#include "ExampleModuleHeader.h"

#include "BaseModule\BaseModule.h"

namespace cube
{
	namespace module
	{
		extern "C" EXAMPLE_MODULE_EXPORT void GetModuleName();

		class ExampleModule : public BaseModule
		{
		public:
			ExampleModule();
			virtual ~ExampleModule();
		};
	}
}