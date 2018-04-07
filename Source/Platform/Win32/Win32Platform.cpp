#ifdef _WIN32

#include "Win32Platform.h"

#include <iostream>
#include <io.h> 
#include <fcntl.h>

#include "Win32DLib.h"
#include "Win32FileSystem.h"

namespace cube
{
	namespace platform
	{
		struct Platform::Data
		{
			HINSTANCE instance;
			HWND window;
			bool isCursorShown = true;
		};

		Platform::Data Platform::data;

		SPtr<FileSystem> Platform::fileSystem;

		PString Platform::title;

		uint32_t Platform::width;
		uint32_t Platform::height;
		uint32_t Platform::windowPosX;
		uint32_t Platform::windowPosY;

		Event<void(KeyCode)> Platform::keyDownEvent;
		Event<void(KeyCode)> Platform::keyUpEvent;
		Event<void(MouseButtonType)> Platform::mouseDownEvent;
		Event<void(MouseButtonType)> Platform::mouseUpEvent;
		Event<void(int)> Platform::mouseWheelEvent;
		Event<void(uint32_t, uint32_t)> Platform::mousePosEvent;

		Event<void()> Platform::loopEvent;
		Event<void(uint32_t, uint32_t)> Platform::resizeEvent;
		Event<void(WindowActivatedState)> Platform::activatedEvent;

		void Platform::Init()
		{
			fileSystem = std::make_shared<Win32FileSystem>();
		}

		void Platform::InitWindow(const String& title, uint32_t width, uint32_t height)
		{
			// Show console if it is debug mode
#ifdef _DEBUG
			if(AllocConsole()) {
				std::wcout.imbue(std::locale(""));

				SetConsoleOutputCP(CP_WINUNICODE);
				_setmode(_fileno(stdout), _O_WTEXT);

				FILE* acStreamIn;
				FILE* acStreamOut;
				FILE* acStreamErr;

				freopen_s(&acStreamIn, "CONIN$", "rb", stdin);
				freopen_s(&acStreamOut, "CONOUT$", "wb", stdout);
				freopen_s(&acStreamErr, "CONOUT$", "wb", stderr);
			}
#endif // _DEBUG

			Platform::title = ToPString(title);
			Platform::width = width;
			Platform::height = height;

			// Register winClass
			WNDCLASSEX winClass;
			winClass.cbSize = sizeof(WNDCLASSEX);
			winClass.style = CS_HREDRAW | CS_VREDRAW;
			winClass.lpfnWndProc = WndProc;
			winClass.cbClsExtra = 0;
			winClass.cbWndExtra = 0;
			winClass.hInstance = data.instance;
			winClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
			winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
			winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			winClass.lpszMenuName = nullptr;
			winClass.lpszClassName = Platform::title.c_str();
			winClass.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

			if(!RegisterClassEx(&winClass))
				std::wcout << L"Win32Platform: Failed to registration while initializing window" << std::endl;
		}

		void Platform::ShowWindow()
		{
			// Create Window
			data.window = CreateWindowEx(0,
				title.c_str(), title.c_str(),
				WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU,
				100, 100, width, height,
				nullptr, nullptr, data.instance, nullptr);

			if(!data.window) {
				std::wcout << L"WinPlatform: Failed to create a window" << std::endl;
			}
		}

		void Platform::StartLoop()
		{
			MSG msg;

			while(1) {
				if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					if(msg.message == WM_QUIT)
						break;
					else {
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				} else {
					loopEvent.Dispatch();
				}
			}
		}

		void Platform::Sleep(uint32_t time)
		{
			::Sleep(time);
		}

		void Platform::ShowCursor()
		{
			if(data.isCursorShown == false) {
				::ShowCursor(TRUE);
				data.isCursorShown = true;
			}
		}

		void Platform::HideCursor()
		{
			if(data.isCursorShown == true) {
				::ShowCursor(FALSE);
				data.isCursorShown = false;
			}
		}

		void Platform::MoveCursor(int x, int y)
		{
			POINT p;
			p.x = x;
			p.y = y;

			ClientToScreen(Platform::data.window, &p);
			SetCursorPos(p.x, p.y);
		}

		void Platform::GetCursorPos(int& x, int& y)
		{
			POINT p;
			::GetCursorPos(&p);

			ScreenToClient(data.window, &p);

			x = p.x;
			y = p.y;
		}

		SPtr<DLib> Platform::LoadDLib(const String& path)
		{
			return std::make_shared<Win32DLib>(path);
		}

		void Win32Platform::Init(HINSTANCE instance)
		{
			data.instance = instance;
		}

		HINSTANCE Win32Platform::GetInstance()
		{
			return Platform::data.instance;
		}

		HWND Win32Platform::GetWindow()
		{
			return Platform::data.window;
		}

		bool isActivatedByMouse = false;

		LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			switch(uMsg) {
				// Clicking the menu buttons on the header doesn't dispatch WA_CLICKACTIVE in WM_ACTIVATE.
				// So using WM_MOUSEACTIVATE, check whether the window is activated by clicking.
				case WM_MOUSEACTIVATE:
					isActivatedByMouse = true;
					break;

				case WM_ACTIVATE:
				{
					WindowActivatedState state;
					WORD s = LOWORD(wParam);

					if(s == WA_ACTIVE)
						state = (isActivatedByMouse == true) ? WindowActivatedState::ClickActive :  WindowActivatedState::Active;
					else if(s == WA_CLICKACTIVE)
						state = WindowActivatedState::ClickActive;
					else if(s == WA_INACTIVE)
						state = WindowActivatedState::Inactive;
					else {
						std::wcout << "Win32Platform: Invalid actiave state (" << s << ")" << std::endl;
					}

					Platform::activatedEvent.Dispatch(state);

					isActivatedByMouse = false;

					break;
				}

				case WM_CLOSE:
					PostQuitMessage(0);
					break;

				case WM_SIZE:
					if(wParam != SIZE_MINIMIZED) {
						Platform::width = lParam & 0xffff;
						Platform::height = (lParam & 0xffff0000) >> 16;

						Platform::resizeEvent.Dispatch(Platform::width, Platform::height);
					}
					break;

				case WM_KEYDOWN:
					Platform::keyDownEvent.Dispatch(static_cast<KeyCode>(wParam));
					break;
				case WM_KEYUP:
					Platform::keyUpEvent.Dispatch(static_cast<KeyCode>(wParam));
					break;

				case WM_LBUTTONDOWN:
					Platform::mouseDownEvent.Dispatch(MouseButtonType::Left);
					break;
				case WM_LBUTTONUP:
					Platform::mouseUpEvent.Dispatch(MouseButtonType::Left);
					break;
				case WM_RBUTTONDOWN:
					Platform::mouseDownEvent.Dispatch(MouseButtonType::Right);
					break;
				case WM_RBUTTONUP:
					Platform::mouseUpEvent.Dispatch(MouseButtonType::Right);
					break;
				case WM_MBUTTONDOWN:
					Platform::mouseDownEvent.Dispatch(MouseButtonType::Middle);
					break;
				case WM_MBUTTONUP:
					Platform::mouseUpEvent.Dispatch(MouseButtonType::Middle);
					break;

				case WM_MOUSEWHEEL:
					Platform::mouseWheelEvent.Dispatch(GET_WHEEL_DELTA_WPARAM(wParam));
					break;

				case WM_MOUSEMOVE:
				{
					int posX = LOWORD(lParam);
					int posY = HIWORD(lParam);

					Platform::mousePosEvent.Dispatch(posX, posY);
					break;
				}
			}

			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	} // namespace platform
} // namespace cube

#endif // _WIN32
