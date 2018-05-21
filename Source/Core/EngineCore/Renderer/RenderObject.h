#pragma once

#include "../EngineCoreHeader.h"

#include "../LogWriter.h"
#include "../GameThread.h"

namespace cube
{
	namespace core
	{
		class RenderObject_RT;

		class ENGINE_CORE_EXPORT RenderObject
		{
		public:
			virtual ~RenderObject(){ }

			static SPtr<RenderObject> Create();

			virtual void Initialize();

			virtual SPtr<RenderObject_RT> CreateRenderObject_RT() const { return nullptr; }
			SPtr<RenderObject_RT> GetRenderObject_RT() const { return mRenderObject_RT; }

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

			SPtr<RenderObject_RT> mRenderObject_RT;
		};

		class ENGINE_CORE_EXPORT RenderObject_RT
		{
		public:
			virtual ~RenderObject_RT(){ }

			virtual void Initialize(){ }

		protected:
			friend class RenderObject;

			RenderObject_RT(){ }
		};

		/*
		// TODO: RenderObjectManager를 만들어서 생성?
		template <typename T, typename ...Args>
		SPtr<T> CreateRenderObject(Args&&... args)
		{
			SPtr<T> ptr = std::make_shared<T>(std::forward<Args>(args)...);
			ptr->_ConstructRenderObject_RT();

			return ptr;
		}
		*/
	} // namespace core
} // namespace cube
