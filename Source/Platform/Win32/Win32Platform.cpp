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
		};

		Platform::Data Platform::data;

		SPtr<FileSystem> Platform::fileSystem;

		PString Platform::title;

		uint32_t Platform::width;
		uint32_t Platform::height;
		uint32_t Platform::windowPosX;
		uint32_t Platform::windowPosY;

		std::function<void(KeyCode)> Platform::keyDownFunction;
		std::function<void(KeyCode)> Platform::keyUpFunction;
		std::function<void(MouseButtonType)> Platform::mouseDownFunction;
		std::function<void(MouseButtonType)> Platform::mouseUpFunction;
		std::function<void(int)> Platform::mouseWheelFunction;
		std::function<void(uint32_t, uint32_t)> Platform::mousePosFunction;

		std::function<void()> Platform::loopFunction;
		std::function<void(uint32_t, uint32_t)> Platform::resizeFunction;

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
					loopFunction();
				}
			}
		}

		void Platform::Sleep(uint32_t time)
		{
			::Sleep(time);
		}

		void Platform::ShowCursor()
		{
			::ShowCursor(TRUE);
		}

		void Platform::HideCursor()
		{
			::ShowCursor(FALSE);
		}

		void Platform::MoveCursor(int x, int y)
		{
			POINT p;
			p.x = x;
			p.y = y;

			ClientToScreen(Platform::data.window, &p);
			SetCursorPos(p.x, p.y);
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

		LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			switch(uMsg) {
				case WM_CLOSE:
					PostQuitMessage(0);
					break;

				case WM_SIZE:
					if(wParam != SIZE_MINIMIZED) {
						Platform::width = lParam & 0xffff;
						Platform::height = (lParam & 0xffff0000) >> 16;

						if(Platform::resizeFunction != nullptr)
							Platform::resizeFunction(Platform::width, Platform::height);
					}
					break;

				case WM_KEYDOWN:
					if(Platform::keyDownFunction != nullptr)
						Platform::keyDownFunction(static_cast<KeyCode>(wParam));
					break;
				case WM_KEYUP:
					if(Platform::keyUpFunction != nullptr)
						Platform::keyUpFunction(static_cast<KeyCode>(wParam));
					break;

				case WM_LBUTTONDOWN:
					if(Platform::mouseDownFunction != nullptr)
						Platform::mouseDownFunction(MouseButtonType::Left);
					break;
				case WM_LBUTTONUP:
					if(Platform::mouseUpFunction != nullptr)
						Platform::mouseUpFunction(MouseButtonType::Left);
					break;
				case WM_RBUTTONDOWN:
					if(Platform::mouseDownFunction != nullptr)
						Platform::mouseDownFunction(MouseButtonType::Right);
					break;
				case WM_RBUTTONUP:
					if(Platform::mouseUpFunction != nullptr)
						Platform::mouseUpFunction(MouseButtonType::Right);
					break;
				case WM_MBUTTONDOWN:
					if(Platform::mouseDownFunction != nullptr)
						Platform::mouseDownFunction(MouseButtonType::Middle);
					break;
				case WM_MBUTTONUP:
					if(Platform::mouseUpFunction != nullptr)
						Platform::mouseUpFunction(MouseButtonType::Middle);
					break;

				case WM_MOUSEWHEEL:
					if(Platform::mouseWheelFunction != nullptr)
						Platform::mouseWheelFunction(GET_WHEEL_DELTA_WPARAM(wParam));
					break;

				case WM_MOUSEMOVE:
				{
					int posX = LOWORD(lParam);
					int posY = HIWORD(lParam);

					if(Platform::mousePosFunction != nullptr)
						Platform::mousePosFunction(posX, posY);
					break;
				}
			}

			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	} // namespace platform
} // namespace cube

#endif // _WIN32
