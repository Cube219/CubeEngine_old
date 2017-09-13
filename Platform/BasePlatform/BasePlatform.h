#pragma once

#include "Base\KeyCode.h"
#include "Base\BaseTypes.h"

#include "BasePlatformDLib.h"

#include <functional>
using namespace std::placeholders;

namespace cube
{
	namespace platform
	{
		class BasePlatform
		{
		public:
			BasePlatform(){ }
			virtual ~BasePlatform(){ }

			virtual UPtr<BasePlatformDLib> LoadDLib(WString path) = 0;

			virtual void InitWindow(WString title, uint32_t width, uint32_t height) = 0;
			virtual void ShowWindow() = 0;

			virtual void StartLoop() = 0;
			virtual void Sleep(uint32_t time) = 0;

			uint32_t GetWindowWidth() const;
			uint32_t GetWindowHeight() const;

			void SetKeyDownFunction(std::function<void(KeyCode)> pFunction);
			void SetKeyUpFunction(std::function<void(KeyCode)> pFunction);
			void SetMouseDownFunction(std::function<void(MouseButtonType)> pFunction);
			void SetMouseUpFunction(std::function<void(MouseButtonType)> pFunction);
			void SetMouseWheelFunction(std::function<void(int)> pFunction);
			void SetMousePosFunction(std::function<void(uint32_t, uint32_t)> pFunction);

			void SetLoopFunction(std::function<void()> pFunction);
			void SetResizeFunction(std::function<void(uint32_t, uint32_t)> pFunction);

		protected:
			std::wstring mTitle;

			uint32_t mWidth;
			uint32_t mHeight;

			std::function<void(KeyCode)> mKeyDownFunction;
			std::function<void(KeyCode)> mKeyUpFunction;
			std::function<void(MouseButtonType)> mMouseDownFunction;
			std::function<void(MouseButtonType)> mMouseUpFunction;
			std::function<void(int)> mMouseWheelFunction;
			std::function<void(uint32_t, uint32_t)> mMousePosFunction;

			std::function<void()> mLoopFunction;
			std::function<void(uint32_t, uint32_t)> mResizeFunction;
		};

		inline uint32_t BasePlatform::GetWindowWidth() const
		{
			return mWidth;
		}
		inline uint32_t BasePlatform::GetWindowHeight() const
		{
			return mHeight;
		}

		inline void BasePlatform::SetKeyDownFunction(std::function<void(KeyCode)> pFunction)
		{
			mKeyDownFunction = pFunction;
		}
		inline void BasePlatform::SetKeyUpFunction(std::function<void(KeyCode)> pFunction)
		{
			mKeyUpFunction = pFunction;
		}
		inline void BasePlatform::SetMouseDownFunction(std::function<void(MouseButtonType)> pFunction)
		{
			mMouseDownFunction = pFunction;
		}
		inline void BasePlatform::SetMouseUpFunction(std::function<void(MouseButtonType)> pFunction)
		{
			mMouseUpFunction = pFunction;
		}
		inline void BasePlatform::SetMouseWheelFunction(std::function<void(int)> pFunction)
		{
			mMouseWheelFunction = pFunction;
		}
		inline void BasePlatform::SetMousePosFunction(std::function<void(uint32_t, uint32_t)> pFunction)
		{
			mMousePosFunction = pFunction;
		}

		inline void BasePlatform::SetLoopFunction(std::function<void()> pFunction)
		{
			mLoopFunction = pFunction;
		}
		inline void BasePlatform::SetResizeFunction(std::function<void(uint32_t, uint32_t)> pFunction)
		{
			mResizeFunction = pFunction;
		}
	}
}