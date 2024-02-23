#pragma once

#include <jo\Jo.hpp>
#include "..\utils\LoaderUtil.hpp"

#include "Mesh3D.hpp"

/** @brief Game objects
 */
namespace Objects
{
	/** @brief Model object
	 */
	class Model
	{
	private:
		/** @brief Model file header
		 */
		struct ModelHeader
		{
			/** @brief Number of meshes inside the model file
			 */
			size_t MeshCount;

			/** @brief Number of textures inside the mesh file
			 */
			size_t TextureCount;
		};

		/** @brief Texture header, textures are always RGB1555
		 */
		struct TextureHeader
		{
			/** @brief Width of the texture
			 */
			uint16_t Width;

			/** @brief Height of the texture
			 */
			uint16_t Height;

			size_t LoadSize() const
			{
				return sizeof(TextureHeader) + (sizeof(jo_color) * (Width * Height));
			}

			jo_color* Data() const
			{
				return (jo_color*)(((char*)this) + sizeof(TextureHeader));
			}
		};

		/** @brief Mesh data header
		 */
		struct MeshHeader
		{
			/** @brief Number of points in the mesh
			 */
			size_t PointCount;

			/** @brief Number of polygons in the mesh
			 */
			size_t PolygonCount;
		};

		/** @brief Face attributes
		 */
		struct Attribute
		{
			/** @brief Indicates whether a texture is applied to this polygon
			 */
			uint8_t HasTexture : 1;

			/** @brief Indicates whether this polygon has a mesh effect applied to it
			 */
			uint8_t HasMeshEffect : 1;

			/** @brief Reserved for future use
			 */
			uint8_t ReservedForFlags : 6;

			/** @brief Reserved for future use
			 */
			uint8_t Reserved;

			/** @brief This field is set if HasTexture field is false
			 */
			jo_color BaseColor;

			/** @brief Index of a texture to use if HasTexture field is true
			 */
			int32_t Texture;
		};

		/** @brief Loaded mesh data
		 */
		Mesh3D* meshes;

		/** @brief Number of loaded meshes
		 */
		size_t meshCount;

		/** @brief Index of first loaded texture
		 */
		int32_t startTextureIndex;

		/** @brief Number of loaded textures
		 */
		size_t textureCount;

	public:
		/** @brief Initializes a new model object from a file
		 * @param modelFile Model file
		 */
		Model(const char* modelFile)
		{
			char* fileBuffer = jo_fs_read_file_in_dir(modelFile, JO_ROOT_DIR, NULL);
			char* iterator = fileBuffer;

			uint16_t lastTextureIndex = jo_sprite_count();
			ModelHeader* header = GetAndIterate<ModelHeader>(iterator);

			// Set defaults
			this->startTextureIndex = -1;
			this->textureCount = header->TextureCount;
			this->meshCount = header->MeshCount;

			this->meshes = new Mesh3D[this->meshCount];

			for (size_t meshIndex = 0; meshIndex < this->meshCount; meshIndex++)
			{
				// Get mesh header
				MeshHeader* meshHeader = GetAndIterate<MeshHeader>(iterator);

				auto mesh = Mesh3D
				(
					meshHeader->PointCount,
					GetAndIterate<POINT>(iterator, meshHeader->PointCount),
					meshHeader->PolygonCount,
					GetAndIterate<POLYGON>(iterator, meshHeader->PolygonCount)
				);

				// Scale mesh
				for (size_t pointIndex = 0; pointIndex < meshHeader->PointCount; pointIndex++)
				{
					mesh.PointTable()[pointIndex] <<= 3;
				}

				for (size_t attributeIndex = 0; attributeIndex < meshHeader->PolygonCount; attributeIndex++)
				{
					// Read mesh attributes
					Attribute* attributeHeader = GetAndIterate<Attribute>(iterator);

					// Set attributes
					uint16_t textureIndex = No_Texture;
					uint16_t color = attributeHeader->BaseColor;

					if (attributeHeader->HasTexture)
					{
						textureIndex = lastTextureIndex + attributeHeader->Texture;
						color = No_Palet;
					}

					#pragma GCC diagnostic push
					#pragma GCC diagnostic ignored "-Wnarrowing"
					ATTR sglAttribute = ATTRIBUTE(
						Dual_Plane,
						SORT_CEN,
						textureIndex,
						color,
						CL32KRGB | No_Gouraud,
						MESHoff | CL32KRGB,
						(attributeHeader->HasTexture != 0 ? sprNoflip : sprPolygon),
						UseLight);

					if (attributeHeader->HasMeshEffect)
					{
        				JO_ADD_FLAG(sglAttribute.atrb, MESHon);
					}
					#pragma GCC diagnostic pop


					mesh.attbl[attributeIndex] = sglAttribute;
				}

				this->meshes[meshIndex] = std::move(mesh);
			}

			// Load textures
			for (size_t textureIndex = 0; textureIndex < textureCount; textureIndex++)
			{
				// Get header
				TextureHeader* textureHeader = GetAndIterate<TextureHeader>(iterator);

				// Get texture data
				jo_img img = { textureHeader->Width, textureHeader->Height, textureHeader->Data() };
				int32_t spriteIndex = jo_sprite_add(&img);

				if (this->startTextureIndex < 0)
				{
					this->startTextureIndex = spriteIndex;
				}
			}

			// Free the read file
			jo_free(fileBuffer);
		}

		/** @brief Destroy the Model object and free its resources, textures must be freed separately
		 */
		~Model()
		{
			delete[] meshes;
			meshCount = 0;
		}

		/** @brief Draw specified mesh
		 * @param mesh Mesh index
		 */
		void Draw(size_t mesh)
		{
			if (mesh < meshCount)
			{
				jo_3d_mesh_draw(meshes[mesh].JoPtr());
			}
		}

		/** @brief Draw all loaded meshes
		 */
		void Draw()
		{
			for (size_t mesh = 0; mesh < meshCount; mesh++)
			{
				jo_3d_mesh_draw(meshes[mesh].JoPtr());
			}
		}

		/** @brief Get index of the first texture loaded
		 * @return Index of first texture or -1 if model has no textures
		 */
		constexpr int32_t GetFirstTextureIndex()
		{
			return this->startTextureIndex;
		}

		/** @brief Gets number of loaded meshes
		 * @return Number of loaded meshes
		 */
		constexpr size_t GetMeshCount()
		{
			return this->meshCount;
		}
	};
}
