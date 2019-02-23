#pragma once

#include "../EngineCoreHeader.h"

#include "../LogWriter.h"
#include "../GameThread.h"

namespace cube
{
	namespace rt
	{
		class RenderObject;
	}
		
	class ENGINE_CORE_EXPORT RenderObject
	{
	public:
		virtual ~RenderObject(){ }

		static SPtr<RenderObject> Create();

		virtual void Initialize();

		virtual SPtr<rt::RenderObject> CreateRenderObject() const { return nullptr; }
		SPtr<rt::RenderObject> GetRenderObject() const { return mRenderObject; }

	protected:
		RenderObject(){ }

		template <typename T>
		void SyncPrimaryData(T& src, T&dst)
		{
			GameThread::QueueTask([&src, &dst]() {
				memcpy(&dst, &src, sizeof(decltype(src)));
			});
		}

		void QueueSyncTask(std::function<void()> syncTaskFunc)
		{
			GameThread::QueueTask(syncTaskFunc);
		}

		SPtr<rt::RenderObject> mRenderObject;
	};

	namespace rt
	{
		class ENGINE_CORE_EXPORT RenderObject
		{
		public:
			virtual ~RenderObject() {}

			virtual void Initialize() {}

		protected:
			friend class RenderObject;

			RenderObject() {}
		};
	} // namespace rt
} // namespace cube
