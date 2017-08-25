#pragma once

#include "EngineCoreHeader.h"
#include "Base\KeyCode.h"
#include "Base\Vector.h"

namespace cube
{
	namespace core
	{
		class InputManager
		{
			friend EngineCore;

		public:
			static bool IsKeyDown(KeyCode keyCode);
			static bool IsKeyUp(KeyCode keyCode);
			static bool IsKeyPressed(KeyCode keyCode);

			static bool IsMouseDown(MouseButtonType buttonType);
			static bool IsMouseUp(MouseButtonType buttonType);
			static bool IsMousePressed(MouseButtonType buttonType);
			static Vector2 GetMousePosition();

		private:
			InputManager() {}

			static void KeyDown(KeyCode keyCode);
			static void KeyUp(KeyCode keyCode);

			static void MouseDown(MouseButtonType buttonType);
			static void MouseUp(MouseButtonType buttonType);
			static void MouseWheel(int wheelDelta);
			static void UpdateMousePos(int posX, int posY);

			static bool isKeyPressed[300];

			static bool isMousePressed[3];
			static Vector2 mousePosition;
		};
	}
}