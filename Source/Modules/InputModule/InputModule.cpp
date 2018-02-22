#include "InputModule.h"

#include "EngineCore/EngineCore.h"
#include "KeyboardMouseInput.h"
#include "XboxControllerInput.h"

namespace cube
{
	namespace module
	{
		InputModule::InputModule()
		{
		}

		InputModule::~InputModule()
		{
			mInfo.name = "InputModule";
		}

		void InputModule::Init(core::EngineCore* eCore, ...)
		{
			SPtr<platform::BasePlatform> platform = eCore->GetPlatform();

			mKMInput = std::make_unique<KeyboardMouseInput>(platform);

			Action jump;
			jump.bindedDigitalButtons.push_back(KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::SpaceBar));
			mActions["Jump"] = jump;

			Action submit;
			submit.bindedDigitalButtons.push_back(KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::Enter));
			mActions["Submit"] = submit;

			Action cancel;
			cancel.bindedDigitalButtons.push_back(KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::Escape));
			mActions["Cancel"] = cancel;

			Axis moveHorizontally;
			moveHorizontally.deadZone = 0.001f;
			moveHorizontally.currentValue = 0.0f;
			moveHorizontally.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::D), 10.0f, 1.0f});
			moveHorizontally.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::A), 10.0f, -1.0f});
			mAxes["MoveHorizontally"] = moveHorizontally;

			Axis moveVertically;
			moveVertically.deadZone = 0.001f;
			moveVertically.currentValue = 0.0f;
			moveVertically.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::W), 10.0f, 1.0f});
			moveVertically.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::S), 10.0f, -1.0f});
			mAxes["MoveVertically"] = moveVertically;

			Axis lookHorizontally;
			lookHorizontally.deadZone = 0.001f;
			lookHorizontally.currentValue = 0.0f;
			lookHorizontally.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::RightArrow), 10.0f, 1.0f});
			lookHorizontally.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::LeftArrow), 10.0f, -1.0f});
			mAxes["LookHorizontally"] = lookHorizontally;

			Axis lookVertically;
			lookVertically.deadZone = 0.001f;
			lookVertically.currentValue = 0.0f;
			lookVertically.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::UpArrow), 10.0f, 1.0f});
			lookVertically.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::DownArrow), 10.0f, -1.0f});
			mAxes["LookVertically"] = lookVertically;
		}

		void InputModule::Update(float dt)
		{
			for(auto& action : mActions) {
				bool isPressed = false;
				
				for(auto& btnInfo : action.second.bindedDigitalButtons) {
					if(btnInfo.getButtonValue() > 0.0f) {
						isPressed = true;
						break;
					}
				}

				action.second.isPressed = isPressed;
			}
			/*
			for(auto& axis : mAxes) {
				for(auto& virtualBtn : axis.second.bindedVirtualButtons) {

				}
			}
			*/
		}

		void InputModule::Destroy()
		{
		}

		bool InputModule::IsActionPressed(String& name)
		{
			return mActions[name].isPressed;
		}

		float InputModule::GetAxisValue(String& name)
		{
			return mAxes[name].currentValue;
		}

		Vector2 InputModule::GetMousePosition()
		{
			return mKMInput->GetMousePosition();
		}
	}
}