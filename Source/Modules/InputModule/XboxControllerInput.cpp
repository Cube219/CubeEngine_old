#include "XboxControllerInput.h"

#include "EngineCore/LogWriter.h"

namespace cube
{
	namespace module
	{
		XboxControllerInput::XboxControllerInput()
		{
		}

		XboxControllerInput::~XboxControllerInput()
		{
		}


#ifdef _WIN32
		bool XboxControllerInput::IsButtonPressed(uint32_t playerIndex, DigitalButton button)
		{
#ifdef _DEBUG
			if(playerIndex >= XUSER_MAX_COUNT) {
				CUBE_LOG(LogType::Error, "Invalid player index (index: {0} / maxIndex: {1})", playerIndex, XUSER_MAX_COUNT - 1);
				return false;
			}
#endif // _DEBUG

			if(mStateAvailable[playerIndex] == false)
				return false;

			WORD padBtn = mState[playerIndex].Gamepad.wButtons;

			switch(button) {
				case XboxControllerInput::X:
					if((padBtn & XINPUT_GAMEPAD_X) > 0)
						return true;
					else
						return false;
				
				case XboxControllerInput::Y:
					if((padBtn & XINPUT_GAMEPAD_Y) > 0)
						return true;
					else
						return false;
				
				case XboxControllerInput::A:
					if((padBtn & XINPUT_GAMEPAD_A) > 0)
						return true;
					else
						return false;
				
				case XboxControllerInput::B:
					if((padBtn & XINPUT_GAMEPAD_B) > 0)
						return true;
					else
						return false;
				
				case XboxControllerInput::Back:
					if((padBtn & XINPUT_GAMEPAD_BACK) > 0)
						return true;
					else
						return false;
				
				case XboxControllerInput::Start:
					if((padBtn & XINPUT_GAMEPAD_START) > 0)
						return true;
					else
						return false;
				
				case XboxControllerInput::LeftButton:
					if((padBtn & XINPUT_GAMEPAD_LEFT_SHOULDER) > 0)
						return true;
					else
						return false;
				
				case XboxControllerInput::RightButton:
					if((padBtn & XINPUT_GAMEPAD_RIGHT_SHOULDER) > 0)
						return true;
					else
						return false;
				
				case XboxControllerInput::DPadLeft:
					if((padBtn & XINPUT_GAMEPAD_DPAD_LEFT) > 0)
						return true;
					else
						return false;
				
				case XboxControllerInput::DPadRight:
					if((padBtn & XINPUT_GAMEPAD_DPAD_RIGHT) > 0)
						return true;
					else
						return false;
				
				case XboxControllerInput::DPadUp:
					if((padBtn & XINPUT_GAMEPAD_DPAD_UP) > 0)
						return true;
					else
						return false;
				
				case XboxControllerInput::DPadDown:
					if((padBtn & XINPUT_GAMEPAD_DPAD_DOWN) > 0)
						return true;
					else
						return false;
				
				case XboxControllerInput::LeftStick:
					if((padBtn & XINPUT_GAMEPAD_LEFT_THUMB) > 0)
						return true;
					else
						return false;
				
				case XboxControllerInput::RightStick:
					if((padBtn & XINPUT_GAMEPAD_RIGHT_THUMB) > 0)
						return true;
					else
						return false;
				
				default:
					CUBE_LOG(LogType::Error, "Invalid digital button ({0})", button);
					return false;
			}

			return false;
		}

		float XboxControllerInput::GetAnalogValue(uint32_t playerIndex, AnalogButton button)
		{
#ifdef _DEBUG
			if(playerIndex >= XUSER_MAX_COUNT) {
				CUBE_LOG(LogType::Error, "Invalid player index (index: {0} / maxIndex: {1})", playerIndex, XUSER_MAX_COUNT - 1);
				return 0.0f;
			}
#endif // _DEBUG

			if(mStateAvailable[playerIndex] == false)
				return 0.0f;

			SHORT stickValue;
			SHORT stickValueAbs;

			BYTE triggerValue;

			switch(button) {
				case XboxControllerInput::LeftStickX:
					stickValue = mState[playerIndex].Gamepad.sThumbLX;
					stickValueAbs = stickValue >= 0 ? stickValue : -stickValue;
					if(stickValue != -32768 && stickValueAbs < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
						stickValue = 0;
					
					return static_cast<float>(stickValue) / 32767.0f;
				
				case XboxControllerInput::LeftStickY:
					stickValue = mState[playerIndex].Gamepad.sThumbLY;
					stickValueAbs = stickValue >= 0 ? stickValue : -stickValue;
					if(stickValue != -32768 && stickValueAbs < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
						stickValue = 0;

					return static_cast<float>(stickValue) / 32767.0f;
				
				case XboxControllerInput::RightStickX:
					stickValue = mState[playerIndex].Gamepad.sThumbRX;
					stickValueAbs = stickValue >= 0 ? stickValue : -stickValue;
					if(stickValue != -32768 && stickValueAbs < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
						stickValue = 0;

					return static_cast<float>(stickValue) / 32767.0f;
				
				case XboxControllerInput::RightStickY:
					stickValue = mState[playerIndex].Gamepad.sThumbRY;
					stickValueAbs = stickValue >= 0 ? stickValue : -stickValue;
					if(stickValue != -32768 && stickValueAbs < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
						stickValue = 0;

					return static_cast<float>(stickValue) / 32767.0f;
				
				case XboxControllerInput::LeftTrigger:
					triggerValue = mState[playerIndex].Gamepad.bLeftTrigger;
					if(triggerValue <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
						triggerValue = 0;
					return static_cast<float>(triggerValue) / 255.0f;
				
				case XboxControllerInput::RightTrigger:
					triggerValue = mState[playerIndex].Gamepad.bRightTrigger;
					if(triggerValue <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
						triggerValue = 0;
					return static_cast<float>(triggerValue) / 255.0f;
				
				default:
					CUBE_LOG(LogType::Error, "Invalid analog button ({0})", button);
					return 0.0f;
			}

			return 0.0f;
		}

		void XboxControllerInput::SetVibration(uint32_t playerIndex, float leftIntensity, float rightIntensity)
		{
#ifdef _DEBUG
			if(playerIndex >= XUSER_MAX_COUNT) {
				CUBE_LOG(LogType::Error, "Invalid player index (index: {0} / maxIndex: {1})", playerIndex, XUSER_MAX_COUNT - 1);
				return;
			}
#endif // _DEBUG

			if(mStateAvailable[playerIndex] == false)
				return;

			XINPUT_VIBRATION vib;
			vib.wLeftMotorSpeed = static_cast<WORD>(65535 * leftIntensity);
			vib.wRightMotorSpeed = static_cast<WORD>(65535 * rightIntensity);

			XInputSetState(playerIndex, &vib);
		}

		void XboxControllerInput::Update()
		{
			for(DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
				DWORD res = XInputGetState(i, &mState[i]);

				if(res != ERROR_SUCCESS) {
					mStateAvailable[i] = false;
					continue;
				}

				mStateAvailable[i] = true;
			}
		}
#else
		// Other platforms doesn't support XboxController
		bool XboxControllerInput::IsButtonPressed(uint32_t playerIndex, DigitalButton button)
		{
			return false;
		}

		float XboxControllerInput::GetAnalogValue(uint32_t playerIndex, AnalogButton button)
		{
			return 0.0f;
		}

		void XboxControllerInput::SetVibration(uint32_t playerIndex, float leftIntensity, float rightIntensity)
		{
		}

		void XboxControllerInput::Update()
		{
		}
#endif // _WIN32
	} // namespace module
} // namespace cube
