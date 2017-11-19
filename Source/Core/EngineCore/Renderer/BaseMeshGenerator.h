#pragma once

#include "../EngineCoreHeader.h"

namespace cube
{
	namespace core
	{
		class BaseMeshGenerator
		{
		public:
			static SPtr<Mesh> GetBoxMesh();

			static SPtr<Mesh> GetCylinderMesh();

			static SPtr<Mesh> GetCapsuleMesh(); // TODO: ���� ����

			static SPtr<Mesh> GetSphereMesh();

			static SPtr<Mesh> GetPlaneMesh();

		private:
			static void SubDivide(SPtr<Mesh>& mesh);
		};
	}
}