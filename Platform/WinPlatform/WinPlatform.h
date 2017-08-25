#pragma once

#include "WinPlatformHeader.h"

#include "BasePlatform\BasePlatform.h"

namespace cube
{
	namespace platform
	{
		class WinPlatform : public BasePlatform
		{
		public:
			WinPlatform(HINSTANCE instance);
			virtual ~WinPlatform();

			UPtr<BasePlatformDLib> LoadDLib(WString path) override;

			void InitWindow(WString title, uint32_t width, uint32_t height) override;
			void ShowWindow() override;
			void StartLoop() override;

			HINSTANCE GetInstance() const
			{
				return mInstance;
			}
			HWND GetWindow() const
			{
				return mWindow;
			}

		private:
			HINSTANCE mInstance;
			HWND mWindow;

			static LRESULT CALLBACK WndProc_s(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		};
	}
}