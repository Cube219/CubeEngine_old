#pragma once

#include "../EngineCoreHeader.h"

#include "../LogWriter.h"
#include "RenderingThread.h"
#include "../Handler.h"

namespace cube
{
	namespace rt
	{
		class RenderObject;
	}
		
	class ENGINE_CORE_EXPORT RenderObject : public Handlable
	{
	public:
		static SPtr<RenderObject> Create();

		virtual ~RenderObject() {}

		Handler<RenderObject> GetHandler() const { return mMyHandler; }

		virtual void Initialize();
		virtual void Destroy();

		virtual SPtr<rt::RenderObject> CreateRenderObject() const { return nullptr; }
		SPtr<rt::RenderObject> GetRenderObject() const { return mRenderObject; }

	protected:
		RenderObject(){ }

		template <typename T>
		void SyncPrimaryData(T& src, T&dst)
		{
			RenderingThread::QueueSyncTask([&src, &dst]() {
				memcpy(&dst, &src, sizeof(decltype(src)));
			});
		}

		void QueueSyncTask(std::function<void()> syncTaskFunc)
		{
			RenderingThread::QueueSyncTask(syncTaskFunc);
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
			virtual void Destroy() {}

		protected:
			friend class cube::RenderObject;

			RenderObject() {}
		};
	} // namespace rt
} // namespace cube
