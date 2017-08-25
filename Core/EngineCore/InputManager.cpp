#include "InputManager.h"

#include "BasePlatform\BasePlatformLogWriter.h"

namespace cube
{
	namespace core
	{
		bool InputManager::isKeyPressed[300];
		bool InputManager::isMousePressed[3];
		Vector2 InputManager::mousePosition;

		bool InputManager::IsKeyDown(KeyCode keyCode)
		{
			return false;
		}

		bool InputManager::IsKeyUp(KeyCode keyCode)
		{
			return false;
		}

		bool InputManager::IsKeyPressed(KeyCode keyCode)
		{
			return isKeyPressed[SCast(int)(keyCode)];
		}

		bool InputManager::IsMouseDown(MouseButtonType buttonType)
		{
			return false;
		}

		bool InputManager::IsMouseUp(MouseButtonType buttonType)
		{
			return false;
		}

		bool InputManager::IsMousePressed(MouseButtonType buttonType)
		{
			return isMousePressed[SCast(int)(buttonType)];
		}

		Vector2 InputManager::GetMousePosition()
		{
			return mousePosition;
		}

		void InputManager::KeyDown(KeyCode keyCode)
		{
			isKeyPressed[SCast(int)(keyCode)] = true;
		}

		void InputManager::KeyUp(KeyCode keyCode)
		{
			isKeyPressed[SCast(int)(keyCode)] = false;
		}

		void InputManager::MouseDown(MouseButtonType buttonType)
		{
			isMousePressed[SCast(int)(buttonType)] = true;
		}

		void InputManager::MouseUp(MouseButtonType buttonType)
		{
			isMousePressed[SCast(int)(buttonType)] = false;
		}

		void InputManager::MouseWheel(int wheelDelta)
		{
		}

		void InputManager::UpdateMousePos(int posX, int posY)
		{
			mousePosition.x = SCast(float)(posX);
			mousePosition.y = SCast(float)(posY);
		}
	}
}