#pragma once

#include "../EngineCoreHeader.h"

namespace cube
{
	class TaskBuffer
	{
	public:
		TaskBuffer(){ }
		~TaskBuffer()
		{
			Flush();
		}

		void WriteTask(std::function<void()> taskFunction)
		{
			mTasks.push_back(taskFunction);
		}

		void QueueAndFlush(TaskBuffer& buf)
		{
			buf.mTasks.insert(buf.mTasks.end(), mTasks.begin(), mTasks.end());
			mTasks.clear();
		}

		void Flush()
		{
			mTasks.clear();
		}

		void ExecuteAll()
		{
			for(auto& fun : mTasks) {
				fun();
			}
		}

	private:
		Vector<std::function<void()>> mTasks;
	};

} // namespace cube
