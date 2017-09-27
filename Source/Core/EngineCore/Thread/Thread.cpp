#include "Thread.h"

#include "../LogWriter.h"

namespace cube
{
	namespace core
	{
		Thread::Thread(uint32_t poolIndex, std::function<void(uint32_t)> onFinishCallback) :
			mPoolIndex(poolIndex), mIsDestroyed(false),
			mIsRun(false), mRunFunction(nullptr),
			mOnFinishCallback(onFinishCallback)
		{
			mThread = std::thread(std::bind(&Thread::Run, this));
		}

		Thread::~Thread()
		{
		}

		void Thread::Start(std::function<void()> function)
		{
			Lock lock(mMutex);

			if(mIsRun == true) {
				LogWriter::WriteDebugLog(L"Thread: Already started thread.");
				return;
			}
			
			mRunFunction = function;

			mStartNotify.notify_one();
		}

		void Thread::WaitUntilFinished()
		{
			Lock lock(mMutex);

			if(mIsRun == false)
				return;

			mIdleNotify.wait(lock);
		}

		void Thread::Destroy()
		{
			{
				Lock lock(mMutex);
				mIsDestroyed = true;
			}

			{
				Lock lock(mMutex);

				if(mIsRun == true)
					mIdleNotify.wait(lock);

				mStartNotify.notify_one();
			}

			mThread.join();
		}

		bool Thread::IsRun()
		{
			Lock lock(mMutex);

			return mIsRun;
		}

		void Thread::Run()
		{
			while(true) {
				{ // Wait until start
					Lock lock(mMutex);

					mStartNotify.wait(lock);

					if(mIsDestroyed)
						break;

					mIsRun = true;
				}

				mRunFunction();

				{ // Finish
					Lock lock(mMutex);

					mRunFunction = nullptr;
					mIsRun = false;

					mOnFinishCallback(mPoolIndex);

					mIdleNotify.notify_one();
				}
			}
		}
	}
}