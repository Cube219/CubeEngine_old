#pragma once

#include "../EngineCoreHeader.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT BaseMeshGenerator
		{
		public:
			static SPtr<Mesh> GetBoxMesh();

			static SPtr<Mesh> GetCylinderMesh();

			static SPtr<Mesh> GetCapsuleMesh(); // TODO: 차후 구현

			static SPtr<Mesh> GetSphereMesh();

			static SPtr<Mesh> GetPlaneMesh();

		private:
			static void SubDivide(SPtr<Mesh>& mesh);

			static void SetNormalVector(SPtr<Mesh>& mesh);
		};
	}
}