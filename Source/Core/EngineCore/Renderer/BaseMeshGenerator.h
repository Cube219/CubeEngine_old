#pragma once

#include "../EngineCoreHeader.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT BaseMeshGenerator
		{
		public:
			static RPtr<Mesh> GetBoxMesh();

			static RPtr<Mesh> GetCylinderMesh();

			static RPtr<Mesh> GetCapsuleMesh(); // TODO: ���� ����

			static RPtr<Mesh> GetSphereMesh();

			static RPtr<Mesh> GetPlaneMesh();

		private:
			static void SubDivide(Mesh* mesh);

			static void SetNormalVector(Mesh* mesh);

			static RPtr<Mesh> RegisterToResourceManager(Mesh* mesh);

			static uint32_t	nextTempID;
		};
	} // namespace core
} // namespace cube
