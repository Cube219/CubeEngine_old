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

			void SetAmbient(const Vector4& ambient) { mAmbient = ambient; }
			void SetDiffuse(const Vector4& diffuse) { mDiffuse = diffuse; }

			Vector4 GetAmbient() const { return mAmbient; }
			Vector4 GetDiffuse() const { return mDiffuse; }
			Vector3 GetPosition() const { return mPosition; }

		protected:
			friend class RendererManager;

			int mIndex = -1; // Used in RendererManager

			Vector4 mAmbient;
			Vector4 mDiffuse;
			
			Vector3 mPosition;
		};
	} // namespace core
} // namespace cube
