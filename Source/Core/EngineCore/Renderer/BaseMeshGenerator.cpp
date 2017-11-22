#include "BaseMeshGenerator.h"

#include "Mesh.h"
#include "Base/Math.h"

namespace cube
{
	namespace core
	{
		SPtr<Mesh> BaseMeshGenerator::GetBoxMesh()
		{
			auto mesh = std::make_shared<Mesh>();

			Vector<Vertex> vertices;
			Vertex v;
			v.pos = {-0.5f, -0.5f, -0.5f, 1};
			v.color = {1, 0, 0, 1};
			v.texCoord = {0, 0};
			vertices.push_back(v);
			v.pos = {-0.5f, 0.5f, -0.5f, 1};
			v.color = {0, 1, 0, 1};
			v.texCoord = {1, 0};
			vertices.push_back(v);
			v.pos = {0.5f, -0.5f, -0.5f, 1};
			v.color = {0, 0, 0, 1};
			v.texCoord = {0, 1};
			vertices.push_back(v);
			v.pos = {0.5f, 0.5f, -0.5f, 1};
			v.color = {1, 1, 0, 1};
			v.texCoord = {1, 1};
			vertices.push_back(v);
			v.pos = {0.5f, -0.5f, 0.5f, 1};
			v.color = {1, 0, 1, 1};
			v.texCoord = {0, 0};
			vertices.push_back(v);
			v.pos = {0.5f, 0.5f, 0.5f, 1};
			v.color = {0, 1, 1, 1};
			v.texCoord = {1, 0};
			vertices.push_back(v);
			v.pos = {-0.5f, -0.5f, 0.5f, 1};
			v.color = {1, 1, 1, 1};
			v.texCoord = {0, 1};
			vertices.push_back(v);
			v.pos = {-0.5f, 0.5f, 0.5f, 1};
			v.color = {0, 0, 0, 1};
			v.texCoord = {1, 1};
			vertices.push_back(v);

			Vector<Index> indices = {
				0,1,2,  2,1,3 ,  2,3,4,  4,3,5,  4,5,6,  6,5,7,  6,7,0,  0,7,1,  6,0,2,  2,4,6,  3,1,5,  5,1,7
			};

			mesh->SetVertex(vertices);
			mesh->SetIndex(indices);

			SetNormalVector(mesh);

			return std::move(mesh);
		}

		SPtr<Mesh> BaseMeshGenerator::GetCylinderMesh()
		{
			auto mesh = std::make_shared<Mesh>();

			constexpr int sliceCount = 20;

			Vector<Vertex> vertices;
			vertices.resize(sliceCount * 2);

			// Side
			float dTheta = 2.0f * Math::Pi / sliceCount;
			for(int i = 0; i < sliceCount; i++) {
				// Up
				vertices[i * 2].pos.x = 0.5f * Math::Cos(dTheta * i);
				vertices[i * 2].pos.y = 1.0f;
				vertices[i * 2].pos.z = 0.5f * Math::Sin(dTheta * i);
				vertices[i * 2].pos.w = 1.0f;
				vertices[i * 2].color = {1.0f, 1.0f, 1.0f, 1.0f};
				vertices[i * 2].texCoord = {0.0f, 0.0f};

				// Down
				vertices[i * 2 + 1].pos.x = 0.5f * Math::Cos(dTheta * i);
				vertices[i * 2 + 1].pos.y = -1.0f;
				vertices[i * 2 + 1].pos.z = 0.5f * Math::Sin(dTheta * i);
				vertices[i * 2 + 1].pos.w = 1.0f;
				vertices[i * 2 + 1].color = {1.0f, 1.0f, 1.0f, 1.0f};
				vertices[i * 2 + 1].texCoord = {0.0f, 0.0f};
			}

			Vector<Index> indices;
			indices.resize(sliceCount * 6);

			for(int i = 0; i < sliceCount - 1; i++) {
				indices[i * 6] = i * 2;
				indices[i * 6 + 1] = i * 2 + 1;
				indices[i * 6 + 2] = i * 2 + 2;

				indices[i * 6 + 3] = i * 2 + 2;
				indices[i * 6 + 4] = i * 2 + 1;
				indices[i * 6 + 5] = i * 2 + 3;
			}
			indices[(sliceCount - 1) * 6] = (sliceCount - 1) * 2;
			indices[(sliceCount - 1) * 6 + 1] = (sliceCount - 1) * 2 + 1;
			indices[(sliceCount - 1) * 6 + 2] = 0;

			indices[(sliceCount - 1) * 6 + 3] = 0;
			indices[(sliceCount - 1) * 6 + 4] = (sliceCount - 1) * 2 + 1;
			indices[(sliceCount - 1) * 6 + 5] = 1;

			// Top
			Vertex v;
			v.pos.x = 0.0f;
			v.pos.y = 1.0f;
			v.pos.z = 0.0f;
			v.pos.w = 1.0f;
			v.color = {1.0f, 1.0f, 1.0f, 1.0f};
			v.texCoord = {0.0f, 0.0f};
			vertices.push_back(v); // index: sliceCount * 2

			for(int i = 0; i < sliceCount - 1; i++) {
				indices.push_back(sliceCount * 2);
				indices.push_back(i * 2);
				indices.push_back((i + 1) * 2);
			}
			indices.push_back(sliceCount * 2);
			indices.push_back((sliceCount - 1) * 2);
			indices.push_back(0);

			// Bottom
			v.pos.x = 0.0f;
			v.pos.y = -1.0f;
			v.pos.z = 0.0f;
			v.pos.w = 1.0f;
			v.color = {1.0f, 1.0f, 1.0f, 1.0f};
			v.texCoord = {0.0f, 0.0f};
			vertices.push_back(v); // index: sliceCount * 2 + 1

			for(int i = 0; i < sliceCount - 1; i++) {
				indices.push_back(sliceCount * 2 + 1);
				indices.push_back((i + 1) * 2 + 1);
				indices.push_back(i * 2 + 1);
			}
			indices.push_back(sliceCount * 2 + 1);
			indices.push_back(1);
			indices.push_back((sliceCount - 1) * 2 + 1);

			mesh->SetVertex(vertices);
			mesh->SetIndex(indices);

			SetNormalVector(mesh);

			return std::move(mesh);
		}

		SPtr<Mesh> BaseMeshGenerator::GetCapsuleMesh()
		{
			auto mesh = std::make_shared<Mesh>();

			return std::move(mesh);
		}

		SPtr<Mesh> BaseMeshGenerator::GetSphereMesh()
		{
			auto mesh = std::make_shared<Mesh>();

			constexpr int divisionNum = 3;

			// Create icosahedron
			constexpr float x = 0.525731f;
			constexpr float z = 0.850651f;

			Vector<Vertex> vertices;
			Vertex v;
			v.color = {1.0f, 1.0f, 1.0f, 1.0f};
			v.texCoord = {0.0f, 0.0f};

			v.pos = {-x, 0.0f, z, 1.0f};
			vertices.push_back(v);
			v.pos = {x, 0.0f, z, 1.0f};
			vertices.push_back(v);
			v.pos = {-x, 0.0f, -z, 1.0f};
			vertices.push_back(v);
			v.pos = {x, 0.0f, -z, 1.0f};
			vertices.push_back(v);
			v.pos = {0.0f, z, x, 1.0f};
			vertices.push_back(v);
			v.pos = {0.0f, z, -x, 1.0f};
			vertices.push_back(v);
			v.pos = {0.0f, -z, x, 1.0f};
			vertices.push_back(v);
			v.pos = {0.0f, -z, -x, 1.0f};
			vertices.push_back(v);
			v.pos = {z, x, 0.0f, 1.0f};
			vertices.push_back(v);
			v.pos = {-z, x, 0.0f, 1.0f};
			vertices.push_back(v);
			v.pos = {z, -x, 0.0f, 1.0f};
			vertices.push_back(v);
			v.pos = {-z, -x, 0.0f, 1.0f};
			vertices.push_back(v);

			Vector<Index> indices = {
				1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
				1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
				3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
				10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
			};

			mesh->SetVertex(vertices);
			mesh->SetIndex(indices);

			// Divide vertices from 1 to 4
			for(int i = 0; i < divisionNum; i++) {
				SubDivide(mesh);
			}

			// Project onto sphere
			Vector<Vertex>& meshVertices = mesh->GetVertex();
			for(uint64_t i = 0; i < meshVertices.size(); i++) {
				glm::vec3 v;
				v.x = meshVertices[i].pos.x;
				v.y = meshVertices[i].pos.y;
				v.z = meshVertices[i].pos.z;

				auto vLength = glm::length(v);
				v /= vLength;
				v *= 0.5f;

				meshVertices[i].pos.x = v.x;
				meshVertices[i].pos.y = v.y;
				meshVertices[i].pos.z = v.z;
			}

			SetNormalVector(mesh);

			return std::move(mesh);
		}

		SPtr<Mesh> BaseMeshGenerator::GetPlaneMesh()
		{
			auto mesh = std::make_shared<Mesh>();

			Vector<Vertex> vertices;
			Vertex v;
			v.color = {1.0f, 1.0f, 1.0f, 1.0f};
			v.texCoord = {0.0f, 0.0f};

			v.pos = {-5.0f, 0.0f, -5.0f, 1.0f};
			vertices.push_back(v);
			v.pos = {-5.0f, 0.0f, 5.0f, 1.0f};
			vertices.push_back(v);
			v.pos = {5.0f, 0.0f, 5.0f, 1.0f};
			vertices.push_back(v);
			v.pos = {5.0f, 0.0f, -5.0f, 1.0f};
			vertices.push_back(v);

			Vector<Index> indices = {
				1,0,3,  1,3,2
			};

			mesh->SetVertex(vertices);
			mesh->SetIndex(indices);

			SetNormalVector(mesh);

			return std::move(mesh);
		}

		void BaseMeshGenerator::SubDivide(SPtr<Mesh>& mesh)
		{
			Vector<Vertex>& oldVertices = mesh->GetVertex();
			Vector<Index>& oldIndices = mesh->GetIndex();
			Vector<Vertex> newVertices;
			Vector<Index> newIndices;

			//       v1
			//       *
			//      / \
			//     /   \
			//  m0*-----*m1
			//   / \   / \
			//  /   \ /   \
			// *-----*-----*
			// v0    m2     v2

			int numTriangles = (int)oldIndices.size() / 3;
			for(int i = 0; i < numTriangles; i++) {
				Vertex v0 = oldVertices[oldIndices[i * 3]];
				Vertex v1 = oldVertices[oldIndices[i * 3 + 1]];
				Vertex v2 = oldVertices[oldIndices[i * 3 + 2]];

				Vertex m0, m1, m2;
				m0.pos.x = (v0.pos.x + v1.pos.x) / 2.0f;
				m0.pos.y = (v0.pos.y + v1.pos.y) / 2.0f;
				m0.pos.z = (v0.pos.z + v1.pos.z) / 2.0f;
				m0.pos.w = 1.0f;

				m1.pos.x = (v1.pos.x + v2.pos.x) / 2.0f;
				m1.pos.y = (v1.pos.y + v2.pos.y) / 2.0f;
				m1.pos.z = (v1.pos.z + v2.pos.z) / 2.0f;
				m1.pos.w = 1.0f;

				m2.pos.x = (v0.pos.x + v2.pos.x) / 2.0f;
				m2.pos.y = (v0.pos.y + v2.pos.y) / 2.0f;
				m2.pos.z = (v0.pos.z + v2.pos.z) / 2.0f;
				m2.pos.w = 1.0f;

				newVertices.push_back(v0);
				newVertices.push_back(v1);
				newVertices.push_back(v2);
				newVertices.push_back(m0);
				newVertices.push_back(m1);
				newVertices.push_back(m2);

				newIndices.push_back(i * 6);
				newIndices.push_back(i * 6 + 3);
				newIndices.push_back(i * 6 + 5);

				newIndices.push_back(i * 6 + 3);
				newIndices.push_back(i * 6 + 4);
				newIndices.push_back(i * 6 + 5);

				newIndices.push_back(i * 6 + 5);
				newIndices.push_back(i * 6 + 4);
				newIndices.push_back(i * 6 + 2);

				newIndices.push_back(i * 6 + 3);
				newIndices.push_back(i * 6 + 1);
				newIndices.push_back(i * 6 + 4);
			}

			mesh->SetVertex(newVertices);
			mesh->SetIndex(newIndices);
		}

		void BaseMeshGenerator::SetNormalVector(SPtr<Mesh>& mesh)
		{
			Vector<Vertex>& vertices = mesh->GetVertex();
			Vector<Index>& indices = mesh->GetIndex();

			for(auto& v : vertices) {
				v.normal = {0.0f, 0.0f, 0.0f};
			}

			int numTriangles = (int)indices.size() / 3;
			for(int i = 0; i < numTriangles; i++) {
				Index i0 = indices[i * 3];
				Index i1 = indices[i * 3 + 1];
				Index i2 = indices[i * 3 + 2];
				Vertex v0 = vertices[i0];
				Vertex v1 = vertices[i1];
				Vertex v2 = vertices[i2];

				glm::vec3 t0;
				t0.x = v1.pos.x - v0.pos.x;
				t0.y = v1.pos.y - v0.pos.y;
				t0.z = v1.pos.z - v0.pos.z;
				glm::vec3 t1;
				t1.x = v2.pos.x - v0.pos.x;
				t1.y = v2.pos.y - v0.pos.y;
				t1.z = v2.pos.z - v0.pos.z;

				glm::vec3 n = glm::cross(t0, t1);

				vertices[i0].normal += n;
				vertices[i1].normal += n;
				vertices[i2].normal += n;
			}

			for(auto& v : vertices) {
				v.normal = glm::normalize(v.normal);
			}
		}
	}
}