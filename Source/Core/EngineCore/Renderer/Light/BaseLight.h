#pragma once

#include "../../EngineCoreHeader.h"

#include "Base/Vector.h"

namespace cube
{
	namespace core
	{
		class BaseLight
		{
		public:
			BaseLight();
			virtual ~BaseLight();

			void SetColor(const Vector4& color) { mColor = color; }
			void SetPosition(const Vector3& pos) { mPosition = pos; }

			Vector4 GetColor() const { return mColor; }
			Vector3 GetPosition() const { return mPosition; }

		protected:
			friend class RendererManager;

			int mIndex = -1; // Used in RendererManager
			
			Vector4 mColor;
			Vector3 mPosition;
		};
	} // namespace core
} // namespace cube
