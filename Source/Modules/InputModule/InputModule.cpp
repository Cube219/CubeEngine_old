#include "InputModule.h"

#include <cfloat>

#include "EngineCore/EngineCore.h"
#include "KeyboardMouseInput.h"
#include "XboxControllerInput.h"
#include "Base/format.h"
#include "EngineCore/LogWriter.h"

namespace cube
{
	namespace module
	{
		BaseModule* CreateModule()
		{
			return new InputModule();
		}

		InputModule::InputModule()
		{
			mInfo.name = "InputModule";
		}

		InputModule::~InputModule()
		{
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
			moveHorizontally.isClamped = true;
			moveHorizontally.currentValue = 0.0f;
			moveHorizontally.deadZone = 0.001f;
			moveHorizontally.sensitivityToZero = 10.0f;
			moveHorizontally.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::D), 10.0f, 1.0f});
			moveHorizontally.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::A), 10.0f, -1.0f});
			mAxes["MoveHorizontally"] = moveHorizontally;

			Axis moveVertically;
			moveVertically.isClamped = true;
			moveVertically.currentValue = 0.0f;
			moveVertically.deadZone = 0.001f;
			moveVertically.sensitivityToZero = 10.0f;
			moveVertically.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::W), 10.0f, 1.0f});
			moveVertically.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::S), 10.0f, -1.0f});
			mAxes["MoveVertically"] = moveVertically;

			Axis lookHorizontally;
			lookHorizontally.isClamped = false;
			lookHorizontally.currentValue = 0.0f;
			lookHorizontally.deadZone = 0.001f;
			lookHorizontally.sensitivityToZero = 10.0f;
			lookHorizontally.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::RightArrow), 10.0f, 1.0f});
			lookHorizontally.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::LeftArrow), 10.0f, -1.0f});
			lookHorizontally.bindedAnalogButtons.push_back(ButtonInfo{[this]() {
				Vector2 mouseDelta = mKMInput->GetMouseDeltaPosition();
				float f[2];
				mouseDelta.GetFloat2(f);
				return f[0];
			}});
			mAxes["LookHorizontally"] = lookHorizontally;

			Axis lookVertically;
			lookVertically.isClamped = false;
			lookVertically.currentValue = 0.0f;
			lookVertically.deadZone = 0.001f;
			lookVertically.sensitivityToZero = 10.0f;
			lookVertically.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::UpArrow), 10.0f, 1.0f});
			lookVertically.bindedVirtualButtons.push_back({KM_DIGIT_BTN_INFO(KeyboardMouseInput::DigitalButton::DownArrow), 10.0f, -1.0f});
			lookVertically.bindedAnalogButtons.push_back(ButtonInfo{[this]() {
				Vector2 mouseDelta = mKMInput->GetMouseDeltaPosition();
				float f[2];
				mouseDelta.GetFloat2(f);
				return f[1];
			}});
			mAxes["LookVertically"] = lookVertically;
		}

		void InputModule::Update(float dt)
		{
			mKMInput->UpdateMouseDelta(dt);

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
			
			for(auto& axis : mAxes) {
				float value = 0.0f;

				// Analog button
				for(auto& aBtn : axis.second.bindedAnalogButtons) {
					value += aBtn.getButtonValue();
				}

				// Virtual button
				for(auto& vBtn : axis.second.bindedVirtualButtons) {
					bool isPressed = false;
					float deltaVal = 0.0f;
					float vBtnCurrentVal = vBtn.currentValue;
					
					if(vBtn.digitalButton.getButtonValue() > 0.0f) {
						isPressed = true;
						deltaVal += vBtn.sensitivity * vBtn.scale * dt;
					}

					if(vBtnCurrentVal != 0.0f && isPressed == false) {
						// Go to zero
						float sign = vBtnCurrentVal > 0.0f ? -1.0f : 1.0f;
						deltaVal = axis.second.sensitivityToZero * sign * dt;

						if((sign < 0.0f && vBtnCurrentVal + deltaVal < 0.0f) || (sign > 0.0f && vBtnCurrentVal + deltaVal > 0.0f)) {
							deltaVal = -vBtnCurrentVal;
						}
					}

					vBtnCurrentVal += deltaVal;

					if(vBtnCurrentVal > 1.0f)
						vBtnCurrentVal = 1.0f;
					else if(vBtnCurrentVal < -1.0f)
						vBtnCurrentVal = -1.0f;

					float vBtnCurrentValAbs = vBtnCurrentVal > 0.0f ? vBtnCurrentVal : -vBtnCurrentVal;
					if(vBtnCurrentValAbs != 0.0f && vBtnCurrentValAbs <= axis.second.deadZone) {
						vBtnCurrentVal = 0.0f;
					}

					vBtn.currentValue = vBtnCurrentVal;
					value += vBtnCurrentVal;
				}

				if(axis.second.isClamped == true) {
					if(value > 1.0f)
						value = 1.0f;
					if(value < -1.0f)
						value = -1.0f;
				}

				axis.second.currentValue = value;
			}
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