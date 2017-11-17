#pragma once

#include "../../EngineCoreHeader.h"

#include <glm.hpp>

namespace cube
{
	namespace core
	{
		enum class MaterialParameterType
		{
			Data,
			Texture
		};

		struct ENGINE_CORE_EXPORT MaterialParameterInfo
		{
			String name;
			MaterialParameterType type;
			size_t dataSize;
		};

		class ENGINE_CORE_EXPORT Material
		{
		public:
			struct MaterialParameter
			{
				MaterialParameterType type;
				char* data;
				size_t size;
				SPtr<Texture> texture; // Only use when the type is Texture
			};

			Material(const Vector<MaterialParameterInfo>& parameters);
			~Material();

			void SetUpdateBool(bool* b) { mIsUpdated = b; }

			template <typename T>
			void SetParameterData(String& name, T& data);

			template <>
			void SetParameterData(String& name, SPtr<Texture>& texture);

			const Vector<MaterialParameter>& GetParameters() const { return mParameters; }

			uint64_t GetParametersDataSize() const { return mParametersDataSize; }

		private:
			HashMap<String, uint64_t> mParameterIndexLookupMap;

			Vector<MaterialParameter> mParameters;
			char* mParametersData;
			uint64_t mParametersDataSize;

			bool* mIsUpdated; // Pointer of Renderer3D::mIsMaterialDataUpdated
		};
	}
}