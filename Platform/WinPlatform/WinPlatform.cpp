#include "WinPlatform.h"

#include "WinDLib.h"

#include <iostream>

namespace cube
{
	namespace platform
	{
		WinPlatform::WinPlatform(HINSTANCE instance) : 
			mInstance(instance)
		{
		}

		WinPlatform::~WinPlatform()
		{
		}

		UPtr<BasePlatformDLib> WinPlatform::LoadDLib(WString path)
		{
			return std::make_unique<WinDLib>(path);
		}

		void WinPlatform::InitWindow(WString title, uint32_t width, uint32_t height)
		{
			// Show console if it is debug mode
#ifdef _DEBUG
			if(AllocConsole()) {
				std::wcout.imbue(std::locale(""));

				FILE* acStreamIn;
				FILE* acStreamOut;
				FILE* acStreamErr;

				freopen_s(&acStreamIn, "CONIN$", "rb", stdin);
				freopen_s(&acStreamOut, "CONOUT$", "wb", stdout);
				freopen_s(&acStreamErr, "CONOUT$", "wb", stderr);
			}
#endif // _DEBUG

			mTitle = title;
			mWidth = width;
			mHeight = height;

			// Register winClass
			WNDCLASSEX winClass;
			winClass.cbSize = sizeof(WNDCLASSEX);
			winClass.style = CS_HREDRAW | CS_VREDRAW;
			winClass.lpfnWndProc = WndProc_s;
			winClass.cbClsExtra = 0;
			winClass.cbWndExtra = 0;
			winClass.hInstance = mInstance;
			winClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
			winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
			winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			winClass.lpszMenuName = nullptr;
			winClass.lpszClassName = title.c_str();
			winClass.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

			if(!RegisterClassEx(&winClass))
				std::wcout << L"WinPlatform: Failed to registration while createing window" << std::endl;
		}

		void WinPlatform::ShowWindow()
		{
			// Create Window
			mWindow = CreateWindowEx(0,
				mTitle.c_str(), mTitle.c_str(),
				WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU,
				100, 100, mWidth, mHeight,
				nullptr, nullptr, mInstance,
				this); // Pass the class pointer to use member functions / variables

			if(!mWindow) {
				std::wcout << L"WinPlatform: Failed to create a window" << std::endl;
			}
		}

		void WinPlatform::StartLoop()
		{
			MSG msg;

			while(1) {
				PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
				if(msg.message == WM_QUIT)
					break;
				else {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				mLoopFunction();

				RedrawWindow(mWindow, nullptr, nullptr, RDW_INTERNALPAINT);
			}
		}

		LRESULT WinPlatform::WndProc_s(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			WinPlatform *pThis;

			if(uMsg == WM_NCCREATE) {
				// Get the class pointer passed by CreateWindowEx
				pThis = static_cast<WinPlatform*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

				SetLastError(0);
				// Save the class pointer in hWnd
				if(!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis))) {
					if(GetLastError() != 0)
						return FALSE;
				}
			} else {
				// Get the class pointer saved in hWnd
				pThis = reinterpret_cast<WinPlatform*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			}

			if(!pThis) {
				// If the class pointer has not been passed yet, just return
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}

			switch(uMsg) {
				case WM_CLOSE:
					PostQuitMessage(0);
					break;

				case WM_SIZE:
					if(wParam != SIZE_MINIMIZED) {
						pThis->mWidth = lParam & 0xffff;
						pThis->mHeight = (lParam & 0xffff0000) >> 16;

						if(pThis->mResizeFunction != nullptr)
							pThis->mResizeFunction(pThis->mWidth, pThis->mHeight);
					}
					break;

				case WM_KEYDOWN:
					if(pThis->mKeyDownFunction != nullptr)
						pThis->mKeyDownFunction(static_cast<KeyCode>(wParam));
					break;
				case WM_KEYUP:
					if(pThis->mKeyUpFunction != nullptr)
						pThis->mKeyUpFunction(static_cast<KeyCode>(wParam));
					break;

				case WM_LBUTTONDOWN:
					if(pThis->mMouseDownFunction != nullptr)
						pThis->mMouseDownFunction(MouseButtonType::Left);
					break;
				case WM_LBUTTONUP:
					if(pThis->mMouseUpFunction != nullptr)
						pThis->mMouseUpFunction(MouseButtonType::Left);
					break;
				case WM_RBUTTONDOWN:
					if(pThis->mMouseDownFunction != nullptr)
						pThis->mMouseDownFunction(MouseButtonType::Right);
					break;
				case WM_RBUTTONUP:
					if(pThis->mMouseUpFunction != nullptr)
						pThis->mMouseUpFunction(MouseButtonType::Right);
					break;
				case WM_MBUTTONDOWN:
					if(pThis->mMouseDownFunction != nullptr)
						pThis->mMouseDownFunction(MouseButtonType::Middle);
					break;
				case WM_MBUTTONUP:
					if(pThis->mMouseUpFunction != nullptr)
						pThis->mMouseUpFunction(MouseButtonType::Middle);
					break;

				case WM_MOUSEWHEEL:
					if(pThis->mMouseWheelFunction != nullptr)
						pThis->mMouseWheelFunction(GET_WHEEL_DELTA_WPARAM(wParam));
					break;

				case WM_MOUSEMOVE:
				{
					int posX = LOWORD(lParam);
					int posY = HIWORD(lParam);

					if(pThis->mMousePosFunction != nullptr)
						pThis->mMousePosFunction(posX, posY);
					break;
				}
			}

			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
}