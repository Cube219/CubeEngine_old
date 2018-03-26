#pragma once

#include "PlatformHeader.h"

#include "Base/KeyCode.h"

#include "PlatformString.h"
#include "DLib.h"
#include "FileSystem.h"

#include <functional>
using namespace std::placeholders;

namespace cube
{
	namespace platform
	{
		class PLATFORM_EXPORT Platform
		{
		public:
			static void Init();

			static void InitWindow(const String& title, uint32_t width, uint32_t height);
			static void ShowWindow();

			static void StartLoop();
			static void Sleep(uint32_t time);

			static uint32_t GetWindowWidth() { return width; }
			static uint32_t GetWindowHeight() { return height; }

			static SPtr<DLib> LoadDLib(const String& path);
			static SPtr<FileSystem> GetFileSystem() { return fileSystem; }

			// TODO: C#의 Event처럼 여러 함수들을 등록할 수 있게 하기?
			static void SetKeyDownFunction(std::function<void(KeyCode)> pFunction)
			{
				keyDownFunction = pFunction;
			}
			static void SetKeyUpFunction(std::function<void(KeyCode)> pFunction)
			{
				keyUpFunction = pFunction;
			}
			static void SetMouseDownFunction(std::function<void(MouseButtonType)> pFunction)
			{
				mouseDownFunction = pFunction;
			}
			static void SetMouseUpFunction(std::function<void(MouseButtonType)> pFunction)
			{
				mouseUpFunction = pFunction;
			}
			static void SetMouseWheelFunction(std::function<void(int)> pFunction)
			{
				mouseWheelFunction = pFunction;
			}
			static void SetMousePosFunction(std::function<void(uint32_t, uint32_t)> pFunction)
			{
				mousePosFunction = pFunction;
			}

			static void SetLoopFunction(std::function<void()> pFunction)
			{
				loopFunction = pFunction;
			}
			static void SetResizeFunction(std::function<void(uint32_t, uint32_t)> pFunction)
			{
				resizeFunction = pFunction;
			}

			struct Data;
			static Data data;

		protected:
			static SPtr<FileSystem> fileSystem;

			static PString title;

			static uint32_t width;
			static uint32_t height;

			static std::function<void(KeyCode)> keyDownFunction;
			static std::function<void(KeyCode)> keyUpFunction;
			static std::function<void(MouseButtonType)> mouseDownFunction;
			static std::function<void(MouseButtonType)> mouseUpFunction;
			static std::function<void(int)> mouseWheelFunction;
			static std::function<void(uint32_t, uint32_t)> mousePosFunction;

			static std::function<void()> loopFunction;
			static std::function<void(uint32_t, uint32_t)> resizeFunction;

		public:
			Platform() = delete;
			~Platform() = delete;
		};
	} // namespace platform
} // namespace cube
