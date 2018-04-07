#pragma once

#include "InputModuleHeader.h"

#include "Base/KeyCode.h"
#include "Base/Vector.h"
#include "Base/Event.h"
#include "Platform.h"

namespace cube
{
	namespace module
	{
		class INPUT_MODULE_EXPORT KeyboardMouseInput
		{
		public:
			enum class DigitalButton : int
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

			KeyboardMouseInput();
			~KeyboardMouseInput();

			bool IsButtonPressed(DigitalButton button);

			bool IsKeyPressed(KeyCode keyCode);

			bool IsMouseDown(MouseButtonType buttonType);

			void LockCursor();
			void UnlockCursor();

			Vector2 GetMousePosition();

			Vector2 GetMouseDeltaPosition();

			void UpdateMouseDelta(float dt);

		private:
			void SetMouseActivate();
			void SetMouseInactivate();

			KeyCode GetKeyCode(DigitalButton button);

			EventFunction<void(KeyCode)> mKeyDownEventFunc;
			EventFunction<void(KeyCode)> mKeyUpEventFunc;
			EventFunction<void(MouseButtonType)> mMouseDownEventFunc;
			EventFunction<void(MouseButtonType)> mMouseUpEventFunc;
			EventFunction<void(int)> mMouseWheelEventFunc;
			EventFunction<void(uint32_t, uint32_t)> mMousePosEventFunc;
			EventFunction<void(platform::WindowActivatedState state)> mActivatedEventFunc;

			bool mIsKeyboardActivated;
			bool mIsMouseActivated;

			bool mIsKeyPressed[MaxKeyCode];

			bool mIsMousePressed[3];
			bool mIsCursorLocked;

			Vector2 mLastMousePos;
			Vector2 mMousePos;
			Vector2 mMouseDeltaPos;
		};
	} // namespace module
} // namespace cube
