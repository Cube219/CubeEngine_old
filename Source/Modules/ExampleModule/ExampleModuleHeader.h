#pragma once

#include "Base/BaseTypes.h"

namespace cube
{
	namespace module
	{
		class ExampleModule;
	}
}

#ifdef EXAMPLE_MODULE_EXPORTS
#define EXAMPLE_MODULE_EXPORT __declspec(dllexport) 
#else // EXAMPLE_MODULE_EXPORTS
#define EXAMPLE_MODULE_EXPORT __declspec(dllimport) 
#endif // EXAMPLE_MODULE_EXPORTS