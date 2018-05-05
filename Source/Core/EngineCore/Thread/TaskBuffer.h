#pragma once

#include "../EngineCoreHeader.h"

namespace cube
{
	class TaskBuffer
	{
	public:
		TaskBuffer(){ }
		~TaskBuffer(){ }

		void WriteTask(std::function<void()> taskFunction)
		{
			mTasks.push_back(taskFunction);
		}
		void Flush()
		{
			mTasks.clear();
		}

	private:
		Vector<std::function<void()>> mTasks;
	};

} // namespace cube
