#pragma once

#include "Base/BaseTypes.h"
#include "Base/KeyCode.h"

#include "DLib.h"
#include "FileSystem.h"

#include <functional>
using namespace std::placeholders;

namespace cube
{
	namespace platform
	{
		class Platform
		{
		public:
			static SPtr<DLib> LoadDLib(const WString& path);
			static SPtr<FileSystem> GetFileSystem();

			static void InitWindow(const WString& title, uint32_t width, uint32_t height);
			static void ShowWindow();

			static void StartLoop();
			static void Sleep(uint32_t time);

			static uint32_t GetWindowWidth();
			static uint32_t GetWindowHeight();

			// TODO: C#의 Event처럼 여러 함수들을 등록할 수 있게 하기?
			static void SetKeyDownFunction(std::function<void(KeyCode)> pFunction);
			static void SetKeyUpFunction(std::function<void(KeyCode)> pFunction);
			static void SetMouseDownFunction(std::function<void(MouseButtonType)> pFunction);
			static void SetMouseUpFunction(std::function<void(MouseButtonType)> pFunction);
			static void SetMouseWheelFunction(std::function<void(int)> pFunction);
			static void SetMousePosFunction(std::function<void(uint32_t, uint32_t)> pFunction);

			static void SetLoopFunction(std::function<void()> pFunction);
			static void SetResizeFunction(std::function<void(uint32_t, uint32_t)> pFunction);

		public:
			Platform() = delete;
			~Platform() = delete;

		private:
			struct Data;

			static Data* data;
		};
	}
}
