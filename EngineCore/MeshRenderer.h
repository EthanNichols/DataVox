#pragma once

#include "Mesh.h"
#include "Material.h"

namespace Component
{
	struct MeshRenderer
	{
	public:

		MeshRenderer()
		{
			mesh = nullptr;
		}
		MeshRenderer(Mesh* mesh/*, Material material*/) :
			mesh(mesh)
			//material(&material)
		{
		}
		~MeshRenderer()
		{
		}

	public:

		template<class Archive>
		inline void serialize(Archive& archive)
		{
			// Create a new Mesh. Which is deleted when the model is actually loaded
			if (mesh == nullptr)
			{
				mesh = new Mesh();
			}
			archive(
				cereal::make_nvp("MeshFilePath", mesh->FilePath)
				//cereal::make_nvp("MaterialFilePath", material->filePath.)
			);
		}

	public:

		Mesh* mesh;
		//const Material* material;
	};
}