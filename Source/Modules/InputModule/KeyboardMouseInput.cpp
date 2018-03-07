#include "KeyboardMouseInput.h"

namespace cube
{
	namespace module
	{
		KeyboardMouseInput::KeyboardMouseInput(SPtr<platform::BasePlatform>& platform) : 
			mPlatform(platform)
		{
			platform->SetKeyDownFunction([this](KeyCode keyCode) {
				mIsKeyPressed[SCast(int)(keyCode)] = true;
			});
			platform->SetKeyUpFunction([this](KeyCode keyCode) {
				mIsKeyPressed[SCast(int)(keyCode)] = false;
			});
			
			platform->SetMouseDownFunction([this](MouseButtonType buttonType) {
				mIsMousePressed[SCast(int)(buttonType)] = true;
			});
			platform->SetMouseUpFunction([this](MouseButtonType buttonType) {
				mIsMousePressed[SCast(int)(buttonType)] = false;
			});
			platform->SetMouseWheelFunction([this](int wheelDelta) {
			});
			platform->SetMousePosFunction([this](int x, int y) {
				mMousePos = Vector2(SCast(float)(x), SCast(float)(y));
			});
		}

		KeyboardMouseInput::~KeyboardMouseInput()
		{
			mPlatform->SetKeyDownFunction(0);
			mPlatform->SetKeyUpFunction(0);
			mPlatform->SetMouseDownFunction(0);
			mPlatform->SetMouseUpFunction(0);
			mPlatform->SetMouseWheelFunction(0);
			mPlatform->SetMousePosFunction(0);
		}

		bool KeyboardMouseInput::IsButtonPressed(DigitalButton button)
		{
			KeyCode code = GetKeyCode(button);
			return mIsKeyPressed[SCast(int)(code)];
		}

		bool KeyboardMouseInput::IsKeyPressed(KeyCode keyCode)
		{
			return mIsKeyPressed[SCast(int)(keyCode)];
		}

		bool KeyboardMouseInput::IsMouseDown(MouseButtonType buttonType)
		{
			return mIsMousePressed[SCast(int)(buttonType)];
		}

		Vector2 KeyboardMouseInput::GetMousePosition()
		{
			return mMousePos;
		}

		KeyCode KeyboardMouseInput::GetKeyCode(DigitalButton button)
		{
			KeyCode keyCode;

			switch(button) {
				case DigitalButton::A:
					keyCode = KeyCode::A;
					break;
				case DigitalButton::B:
					keyCode = KeyCode::B;
					break;
				case DigitalButton::C:
					keyCode = KeyCode::C;
					break;
				case DigitalButton::D:
					keyCode = KeyCode::D;
					break;
				case DigitalButton::E:
					keyCode = KeyCode::E;
					break;
				case DigitalButton::F:
					keyCode = KeyCode::F;
					break;
				case DigitalButton::G:
					keyCode = KeyCode::G;
					break;
				case DigitalButton::H:
					keyCode = KeyCode::H;
					break;
				case DigitalButton::I:
					keyCode = KeyCode::I;
					break;
				case DigitalButton::J:
					keyCode = KeyCode::J;
					break;
				case DigitalButton::K:
					keyCode = KeyCode::K;
					break;
				case DigitalButton::L:
					keyCode = KeyCode::L;
					break;
				case DigitalButton::M:
					keyCode = KeyCode::M;
					break;
				case DigitalButton::N:
					keyCode = KeyCode::N;
					break;
				case DigitalButton::O:
					keyCode = KeyCode::O;
					break;
				case DigitalButton::P:
					keyCode = KeyCode::P;
					break;
				case DigitalButton::Q:
					keyCode = KeyCode::Q;
					break;
				case DigitalButton::R:
					keyCode = KeyCode::R;
					break;
				case DigitalButton::S:
					keyCode = KeyCode::S;
					break;
				case DigitalButton::T:
					keyCode = KeyCode::T;
					break;
				case DigitalButton::U:
					keyCode = KeyCode::U;
					break;
				case DigitalButton::V:
					keyCode = KeyCode::V;
					break;
				case DigitalButton::W:
					keyCode = KeyCode::W;
					break;
				case DigitalButton::X:
					keyCode = KeyCode::X;
					break;
				case DigitalButton::Y:
					keyCode = KeyCode::Y;
					break;
				case DigitalButton::Z:
					keyCode = KeyCode::Z;
					break;
				case DigitalButton::Tab:
					keyCode = KeyCode::Tab;
					break;
				case DigitalButton::Enter:
					keyCode = KeyCode::Enter;
					break;
				case DigitalButton::Pause:
					keyCode = KeyCode::Pause;
					break;
				case DigitalButton::CapsLock:
					keyCode = KeyCode::CapsLock;
					break;
				case DigitalButton::Escape:
					keyCode = KeyCode::Escape;
					break;
				case DigitalButton::SpaceBar:
					keyCode = KeyCode::Space;
					break;
				case DigitalButton::PageUp:
					// TODO: 추가
					break;
				case DigitalButton::PageDown:
					// TODO: 추가
					break;
				case DigitalButton::End:
					keyCode = KeyCode::End;
					break;
				case DigitalButton::Home:
					keyCode = KeyCode::Home;
					break;
				case DigitalButton::LeftArrow:
					keyCode = KeyCode::LeftArrow;
					break;
				case DigitalButton::UpArrow:
					keyCode = KeyCode::UpArrow;
					break;
				case DigitalButton::RightArrow:
					keyCode = KeyCode::RightArrow;
					break;
				case DigitalButton::DownArrow:
					keyCode = KeyCode::DownArrow;
					break;
				case DigitalButton::Insert:
					keyCode = KeyCode::Insert;
					break;
				case DigitalButton::Backspace:
					keyCode = KeyCode::Backspace;
					break;
				case DigitalButton::Delete:
					keyCode = KeyCode::Delete;
					break;
				case DigitalButton::Num0:
					keyCode = KeyCode::Num0;
					break;
				case DigitalButton::Num1:
					keyCode = KeyCode::Num1;
					break;
				case DigitalButton::Num2:
					keyCode = KeyCode::Num2;
					break;
				case DigitalButton::Num3:
					keyCode = KeyCode::Num3;
					break;
				case DigitalButton::Num4:
					keyCode = KeyCode::Num4;
					break;
				case DigitalButton::Num5:
					keyCode = KeyCode::Num5;
					break;
				case DigitalButton::Num6:
					keyCode = KeyCode::Num6;
					break;
				case DigitalButton::Num7:
					keyCode = KeyCode::Num7;
					break;
				case DigitalButton::Num8:
					keyCode = KeyCode::Num8;
					break;
				case DigitalButton::Num9:
					keyCode = KeyCode::Num9;
					break;
				case DigitalButton::LeftShift:
					keyCode = KeyCode::LeftShift;
					break;
				case DigitalButton::RightShift:
					// TODO: 추가
					break;
				case DigitalButton::LeftCtrl:
					keyCode = KeyCode::LeftControl;
					break;
				case DigitalButton::RightCtrl:
					// TODO: 추가
					break;
				case DigitalButton::LeftAlt:
					keyCode = KeyCode::LeftAlt;
					break;
				case DigitalButton::RightAlt:
					// TODO: 추가
					break;
				case DigitalButton::F1:
					keyCode = KeyCode::F1;
					break;
				case DigitalButton::F2:
					keyCode = KeyCode::F2;
					break;
				case DigitalButton::F3:
					keyCode = KeyCode::F3;
					break;
				case DigitalButton::F4:
					keyCode = KeyCode::F4;
					break;
				case DigitalButton::F5:
					keyCode = KeyCode::F5;
					break;
				case DigitalButton::F6:
					keyCode = KeyCode::F6;
					break;
				case DigitalButton::F7:
					keyCode = KeyCode::F7;
					break;
				case DigitalButton::F8:
					keyCode = KeyCode::F8;
					break;
				case DigitalButton::F9:
					keyCode = KeyCode::F9;
					break;
				case DigitalButton::F10:
					keyCode = KeyCode::F10;
					break;
				case DigitalButton::F11:
					keyCode = KeyCode::F11;
					break;
				case DigitalButton::F12:
					keyCode = KeyCode::F12;
					break;
				case DigitalButton::NumPad0:
					keyCode = KeyCode::NumPad0;
					break;
				case DigitalButton::NumPad1:
					keyCode = KeyCode::NumPad1;
					break;
				case DigitalButton::NumPad2:
					keyCode = KeyCode::NumPad2;
					break;
				case DigitalButton::NumPad3:
					keyCode = KeyCode::NumPad3;
					break;
				case DigitalButton::NumPad4:
					keyCode = KeyCode::NumPad4;
					break;
				case DigitalButton::NumPad5:
					keyCode = KeyCode::NumPad5;
					break;
				case DigitalButton::NumPad6:
					keyCode = KeyCode::NumPad6;
					break;
				case DigitalButton::NumPad7:
					keyCode = KeyCode::NumPad7;
					break;
				case DigitalButton::NumPad8:
					keyCode = KeyCode::NumPad8;
					break;
				case DigitalButton::NumPad9:
					keyCode = KeyCode::NumPad9;
					break;

				default:
					break;
			}

			return keyCode;
		}
	}
}