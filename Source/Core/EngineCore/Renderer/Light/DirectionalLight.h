#pragma once

#include "../../EngineCoreHeader.h"

#include "BaseLight.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT DirectionalLight : public BaseLight
		{
		public:
			virtual ~DirectionalLight();

			static SPtr<DirectionalLight> Create();

			virtual SPtr<RenderObject_RT> CreateRenderObject_RT() const override;

			SPtr<DirectionalLight_RT> GetRenderObject_RT() const { return DPCast(DirectionalLight_RT)(mRenderObject_RT); }

			void SetDirection(const Vector3& direction);

			Vector3 GetDirection() const { return mDirection; }

		private:
			DirectionalLight();

			Vector3 mDirection;
		};

		class DirectionalLight_RT : public BaseLight_RT
		{
		public:
			virtual ~DirectionalLight_RT(){ }

			Vector3 GetDirection() const { return mDirection; }

		private:
			friend class DirectionalLight;

			DirectionalLight_RT(){ }

			Vector3 mDirection;
		};
	} // namespace core
} // namespace cube
