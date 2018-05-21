#pragma once

#include "../../EngineCoreHeader.h"

#include "Base/Vector.h"
#include "../RenderObject.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT BaseLight : public RenderObject
		{
		public:
			virtual ~BaseLight();

			static SPtr<BaseLight> Create();

			virtual SPtr<RenderObject_RT> CreateRenderObject_RT() const override;
			SPtr<BaseLight_RT> GetRenderObject_RT() const { return DPCast(BaseLight_RT)(mRenderObject_RT); }

			void SetColor(const Vector4& color);
			void SetPosition(const Vector3& pos);

			Vector4 GetColor() const { return mColor; }
			Vector3 GetPosition() const { return mPosition; }

		protected:
			BaseLight();

			Vector4 mColor;
			Vector3 mPosition;
		};

		class BaseLight_RT : public RenderObject_RT
		{
		public:
			virtual ~BaseLight_RT(){ }

			Vector4 GetColor() const { return mColor; }
			Vector3 GetPosition() const { return mPosition; }

		protected:
			friend class BaseLight;
			friend class RendererManager;

			BaseLight_RT(){ }

			int mIndex = -1; // Used in RendererManager

			Vector4 mColor;
			Vector3 mPosition;
		};
	} // namespace core
} // namespace cube
