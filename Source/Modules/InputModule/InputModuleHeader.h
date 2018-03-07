#pragma once
#pragma warning(disable:4275)
#pragma warning(disable:4251)

#include "Base/BaseTypes.h"

namespace cube
{
	namespace module
	{
		class InputModule;

		class KeyboardMouseInput;
		class XboxControllerInput;
	}
}

#ifdef INPUT_MODULE_EXPORTS
	#define INPUT_MODULE_EXPORT __declspec(dllexport) 
#else // INPUT_MODULE_EXPORTS
	#define INPUT_MODULE_EXPORT __declspec(dllimport) 
#endif // INPUT_MODULE_EXPORTS