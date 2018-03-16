#pragma once

#include "InputModuleHeader.h"

#include "BaseModule/BaseModule.h"

#include "BasePlatform/BasePlatform.h"
#include "Base/Vector.h"

namespace cube
{
	namespace module
	{
		struct ButtonInfo
		{
			std::function<float()> getButtonValue;
		};
#define KM_DIGIT_BTN_INFO(btn) ButtonInfo{[this](){return mKMInput->IsButtonPressed(btn) == true ? 1.0f : 0.0f;}}
#define XBOX_DIGIT_BTN_INFO(playerIdx, btn) ButtonInfo{[this](){return mXboxInput->IsButtonPressed(playerIdx, btn) == true ? 1.0f : 0.0f;}}
#define XBOX_ANALOG_BTN_INFO(playerIdx, btn) ButtonInfo{[this](){return mXboxInput->GetAnalogValue(playerIdx, btn);}}

		struct Action
		{
			bool isPressed;
			Vector<ButtonInfo> bindedDigitalButtons;
		};

		struct Axis
		{
			struct VirtualButtonInfo
			{
				ButtonInfo digitalButton;
				float sensitivity;
				float scale;

				float currentValue = 0.0f;
			};
			float deadZone;
			float sensitivityToZero;

			bool isClamped;
			float currentValue;
			Vector<ButtonInfo> bindedAnalogButtons;
			Vector<VirtualButtonInfo> bindedVirtualButtons;
		};

		extern "C" INPUT_MODULE_EXPORT BaseModule* CreateModule();

		class INPUT_MODULE_EXPORT InputModule : public BaseModule
		{
		public:
			InputModule();
			~InputModule();

			void Init(core::EngineCore* eCore, ...) final override;

			void Update(float dt) final override;

			void Destroy() final override;

			bool IsActionPressed(String& name);

			float GetAxisValue(String& name);

			void SendVibration(uint32_t playerIndex, float time, float intensity);
			void SendVibration(uint32_t playerIndex, float time, float leftIntensity, float rightIntensity);

			Vector2 GetMousePosition();

		private:
			UPtr<KeyboardMouseInput> mKMInput;
			UPtr<XboxControllerInput> mXboxInput;

			HashMap<String, Action> mActions;
			HashMap<String, Axis> mAxes;

			Array<bool, 4> mIsVibrated;
			Array<float, 4> mRemainedVibrationTime;
		};
	}
}
