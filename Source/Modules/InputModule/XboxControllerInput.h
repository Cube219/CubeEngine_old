#pragma once

#include "InputModuleHeader.h"

#ifdef _WIN32
#include <Windows.h>
#include <Xinput.h>
#endif // _WIN32

namespace cube
{
	namespace module
	{
		class XboxControllerInput
		{
		public:
			enum class DigitalButton
			{
				X, Y, A, B, Back, Start, LeftButton, RightButton,
				DPadLeft, DPadRight, DPadUp, DPadDown, LeftStick, RightStick,
				LeftTrigger, RightTrigger, 
				/*LeftStickLeft, LeftStickRight, LeftStickUp, LeftStickDown,
				RightStickLeft, RightStickRight, RightStickUp, RightStickDown*/ // TODO: StickµÈ¿« Digital»≠
			};

			enum class AnalogButton
			{
				LeftStickX, LeftStickY, RightStickX, RightStickY,
				LeftTrigger, RightTrigger
			};

			XboxControllerInput();
			~XboxControllerInput();

			bool IsButtonPressed(uint32_t playerIndex, DigitalButton button);

			float GetAnalogValue(uint32_t playerIndex, AnalogButton button);

			void SetVibration(uint32_t playerIndex, float leftIntensity, float rightIntensity);

			void Update();

		private:
#ifdef _WIN32
			XINPUT_STATE mState[XUSER_MAX_COUNT];
			bool mStateAvailable[XUSER_MAX_COUNT];
#endif
		};
	} // namespace module
} // namespace cube
