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
//#define XBOX_BTN_INFO(btn) ButtonInfo{InputType::XboxController, btn}

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
			};
			float deadZone;
			float sensitivityToZero;

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

			Vector2 GetMousePosition();

		private:
			UPtr<KeyboardMouseInput> mKMInput;

			HashMap<String, Action> mActions;
			HashMap<String, Axis> mAxes;
		};
	}
}

/*
Axes
  MoveHorizontally
  MoveVertically
  AimHorizontally
  AimVertically
  Jump
  Submit
  Cancel


MouseX / Y
MouseLeft / Right / Wheel
Keyboard?

XboxLeftJoystickX / Y
XboxRightJoystickX / Y
XboxLeftTrigger
XboxRightTrigger
XboxLeftButton
XboxRightButton
XboxButtonX
XboxButtonY
XboxButtonA
XboxButtonB
XboxDpadLeft
XboxDpadRight
XboxDpadUp
XboxDpadDown
*/