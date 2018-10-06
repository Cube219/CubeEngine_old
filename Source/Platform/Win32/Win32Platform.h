#pragma once

#ifdef _WIN32

#include "../PlatformHeader.h"

#include <Windows.h>

#include "../Platform.h"

namespace cube
{
	namespace platform
	{
		class PLATFORM_EXPORT Win32Platform : public Platform
		{
		public:
			static void Init(HINSTANCE instance);

			static void InitImpl();

			static void InitWindowImpl(const String& title, uint32_t width, uint32_t height);
			static void ShowWindowImpl();

			static void StartLoopImpl();
			static void FinishLoopImpl();
			static void SleepImpl(uint32_t time);

			static void ShowCursorImpl();
			static void HideCursorImpl();

			static void MoveCursorImpl(int x, int y);
			static void GetCursorPosImpl(int& x, int& y);

			static SPtr<DLib> LoadDLibImpl(const String& path);

			static HINSTANCE GetInstance();
			static HWND GetWindow();

		private:
			friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			static HINSTANCE instance;
			static HWND window;
			static bool isCursorShown;

			Win32Platform() = delete;
			~Win32Platform() = delete;
		};
		PLATFORM_DEFINITION(Win32Platform)

		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}

#endif // _WIN32
