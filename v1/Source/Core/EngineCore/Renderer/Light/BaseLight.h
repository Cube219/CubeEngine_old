#pragma once

#include "../../EngineCoreHeader.h"

#include "Base/Vector.h"
#include "../RenderObject.h"

namespace cube
{
	class ENGINE_CORE_EXPORT BaseLight : public RenderObject
	{
	public:
		virtual ~BaseLight();

		static UPtr<BaseLight> Create();

		virtual SPtr<rt::RenderObject> CreateRenderObject() const override;
		SPtr<rt::BaseLight> GetRenderObject() const { return DPCast(rt::BaseLight)(mRenderObject); }

		void SetColor(const Vector4& color);
		void SetPosition(const Vector3& pos);

		Vector4 GetColor() const { return mColor; }
		Vector3 GetPosition() const { return mPosition; }

	protected:
		BaseLight();

		Vector4 mColor;
		Vector3 mPosition;
	};

	namespace rt
	{
		class BaseLight : public RenderObject
		{
		public:
			virtual ~BaseLight() {}

			Vector4 GetColor() const { return mColor; }
			Vector3 GetPosition() const { return mPosition; }

		protected:
			friend class cube::BaseLight;
			friend class RendererManager;

			BaseLight() {}

			int mIndex = -1; // Used in RendererManager

			Vector4 mColor;
			Vector3 mPosition;
		};
	} // namespace rt
} // namespace cube
