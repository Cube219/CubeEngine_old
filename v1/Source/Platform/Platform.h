#pragma once

#include "PlatformHeader.h"

#include "Base/KeyCode.h"

#include "PlatformString.h"
#include "DLib.h"
#include "FileSystem.h"
#include "Base/Event.h"

using namespace std::placeholders;

namespace cube
{
	namespace platform
	{
		enum class WindowActivatedState
		{
			Active, ClickActive, Inactive
		};

		class PLATFORM_EXPORT Platform
		{
		public:
			static void Init();

			static void InitWindow(StringRef title, Uint32 width, Uint32 height);
			static void ShowWindow();

			static void StartLoop();
			static void FinishLoop();
			static void Sleep(Uint32 time);

			static void ShowCursor();
			static void HideCursor();
			static void MoveCursor(int x, int y);
			static void GetCursorPos(int& x, int& y);

			static uint32_t GetWindowWidth() { return width; }
			static uint32_t GetWindowHeight() { return height; }

			static SPtr<DLib> LoadDLib(StringRef path);

			static auto& GetKeyDownEvent() { return keyDownEvent; }
			static auto& GetKeyUpEvent() { return keyUpEvent; }
			static auto& GetMouseDownEvent() { return mouseDownEvent; }
			static auto& GetMouseUpEvent() { return mouseUpEvent; }
			static auto& GetMouseWheelEvent() { return mouseWheelEvent; }
			static auto& GetMousePosEvent() { return mousePosEvent; }
			static auto& GetLoopEvent() { return loopEvent; }
			static auto& GetResizeEvent() { return resizeEvent; }
			static auto& GetActivatedEvent() { return activatedEvent; }
			static auto& GetClosingEvent() { return closingEvent; }

		protected:
			static PString title;

			static bool isFinished;

			static Uint32 width;
			static Uint32 height;
			static Uint32 windowPosX;
			static Uint32 windowPosY;

			static Event<void(KeyCode)> keyDownEvent;
			static Event<void(KeyCode)> keyUpEvent;
			static Event<void(MouseButtonType)> mouseDownEvent;
			static Event<void(MouseButtonType)> mouseUpEvent;
			static Event<void(int)> mouseWheelEvent;
			static Event<void(Uint32, Uint32)> mousePosEvent;

			static Event<void()> loopEvent;
			static Event<void(Uint32, Uint32)> resizeEvent;
			static Event<void(WindowActivatedState)> activatedEvent;
			static Event<void()> closingEvent;

		public:
			Platform() = delete;
			~Platform() = delete;
		};

#define PLATFORM_DEFINITION(Child)                             \
		inline void Platform::Init() {                         \
			Child::InitImpl();                                 \
		}                                                      \
		                                                       \
		inline void Platform::InitWindow                       \
			(StringRef title, Uint32 width, Uint32 height) {   \
			Child::InitWindowImpl(title, width, height);       \
		}                                                      \
		inline void Platform::ShowWindow() {                   \
			Child::ShowWindowImpl();                           \
		}                                                      \
                                                               \
		inline void Platform::StartLoop() {                    \
			Child::StartLoopImpl();                            \
		}                                                      \
		inline void Platform::FinishLoop() {                   \
			Child::FinishLoopImpl();                           \
		}                                                      \
		inline void Platform::Sleep(Uint32 time) {             \
			Child::SleepImpl(time);                            \
		}                                                      \
                                                               \
		inline void Platform::ShowCursor() {                   \
			Child::ShowCursorImpl();                           \
		}                                                      \
		inline void Platform::HideCursor() {                   \
			Child::HideCursorImpl();                           \
		}                                                      \
		inline void Platform::MoveCursor(int x, int y) {       \
			Child::MoveCursorImpl(x, y);                       \
		}                                                      \
		inline void Platform::GetCursorPos(int& x, int& y) {   \
			Child::GetCursorPosImpl(x, y);                     \
		}                                                      \
                                                               \
		inline SPtr<DLib> Platform::LoadDLib(StringRef path) { \
			return Child::LoadDLibImpl(path);                  \
		}
	} // namespace platform
} // namespace cube
