#pragma once

#include "InputModuleHeader.h"

#include "Base/KeyCode.h"
#include "BasePlatform/BasePlatform.h"
#include "Base/Vector.h"

namespace cube
{
	namespace module
	{
		class INPUT_MODULE_EXPORT KeyboardMouseInput
		{
		public:
			enum DigitalButton
			{
				MouseLeft, MouseRight, MoudeMiddle,
				A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
				Tab, Enter, Pause, CapsLock, Escape, SpaceBar, PageUp, PageDown, End, Home,
				LeftArrow, UpArrow, RightArrow, DownArrow, Insert, Backspace, Delete,
				Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
				LeftShift, RightShift, LeftCtrl, RightCtrl, LeftAlt, RightAlt,
				F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
				NumPad0, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7, NumPad8, NumPad9
			};

			KeyboardMouseInput(SPtr<platform::BasePlatform>& platform);
			~KeyboardMouseInput();

			bool IsButtonPressed(DigitalButton button);

			bool IsKeyPressed(KeyCode keyCode);

			bool IsMouseDown(MouseButtonType buttonType);

			Vector2 GetMousePosition();

		private:
			KeyCode GetKeyCode(DigitalButton button);

			bool mIsKeyPressed[MaxKeyCode];

			bool mIsMousePressed[3];

			Vector2 mMousePos;

			SPtr<platform::BasePlatform> mPlatform;
		};
	}
}