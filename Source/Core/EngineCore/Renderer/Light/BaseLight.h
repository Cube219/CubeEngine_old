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

			void SetAmbient(Vector4& ambient) { mAmbient = ambient; }
			void SetDiffuse(Vector4& diffuse) { mDiffuse = diffuse; }
			void SetSpecular(Vector4& specular) { mSpecular = specular; }
			void SetPosition(Vector3& position) { mPosition = position; }

			Vector4 GetAmbient() const { return mAmbient; }
			Vector4 GetDiffuse() const { return mDiffuse; }
			Vector4 GetSpecular() const { return mSpecular; }
			Vector3 GetPosition() const { return mPosition; }

		protected:
			Vector4 mAmbient;
			Vector4 mDiffuse;
			Vector4 mSpecular;
			
			Vector3 mPosition;
		};
	}
}
