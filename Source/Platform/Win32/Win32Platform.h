#pragma once

#include "../Platform.h"

#include <Windows.h>

namespace cube
{
	namespace platform
	{
		class Win32Platform : public Platform
		{
		public:
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
