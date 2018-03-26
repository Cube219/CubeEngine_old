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

			static HINSTANCE GetInstance();
			static HWND GetWindow();

		private:
			friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			Win32Platform() = delete;
			~Win32Platform() = delete;
		};

		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}

#endif // _WIN32
