#include "Thread.h"

#include "../LogWriter.h"

namespace cube
{
	Thread::Thread(Uint32 poolIndex, std::function<void(Uint32)> onFinishCallback) :
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
			CUBE_LOG(LogType::Warning, "Already started thread.");
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
} // namespace cube
