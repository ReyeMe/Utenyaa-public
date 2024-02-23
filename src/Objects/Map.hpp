#pragma once

#include <jo\Jo.hpp>
#include "Mesh3D.hpp"
#include "..\utils\LoaderUtil.hpp"
#include "..\utils\std\vector.h"
#include "..\Interfaces\IColliding.hpp"

/** @brief Game objects
 */
namespace Objects
{
	/** @brief Map object
	 */
	class Map
	{
	public:

		/** @brief Size of the map
		 */
		static const int MapDimensionSize = 20;

	private:
		/** @brief Index of first color in gouraud table
		 */
		static const int GouraudTableStart = JO_VDP1_VRAM + 0x70000;

		/** @brief Level tile
		 */
		struct Tile
		{
			/** @brief Depth and rotation are present in a single byte
			 */
			unsigned char Rotation : 2;

			/** @brief Depth and rotation are present in a single byte
			 */
			unsigned char Depth : 6;

			/** @brief Index of a texture to use
			 */
			unsigned char Texture;

			/** @brief Unused space
			 */
			unsigned short Dummy;
		};

		/** @brief Gouraud table entry
		 */
		struct GouraudColor
		{
			/** @brief Vertex color
			 */
			jo_color Colors[4];
		};

		/** @brief Base entity definition as saved within a file
		 */
		struct EntityDefinition
		{
			/** @brief Entity type to spawn
			 */
			int Type;

			/** @brief X coordinate of tile to spawn entity on
			 */
			unsigned short TileX;

			/** @brief Y coordinate of tile to spawn entity on
			 */
			unsigned short TileY;

			/** @brief Direction in radians
			 */
			Fxp Direction;

			/** @brief Unused space
			 */
			unsigned char Dummy[16];
		};

		/** @brief Light data
		 */
		struct Light
		{
			/** @brief Light direction
			 */
			Vec3 Direction;

			/** @brief Light color
			 */
			jo_color Color;

			/** @brief Unused space
			 */
			short Dummy;
		};

		/** @brief Level file data
		 */
		struct LevelData
		{
			/** @brief File identifier (should read 'UTE' and 4th byte indicates version)
			 */
			unsigned char Identifier[4];

			/** @brief Map tiles
			 */
			Tile TileData[Map::MapDimensionSize * Map::MapDimensionSize];

			/** @brief Level sun
			 */
			Light Sun;

			/** @brief Precalculated gouraud table
			 */
			GouraudColor Gouraud[Map::MapDimensionSize * Map::MapDimensionSize];

			/** @brief Precalculated face normals
			 */
			Vec3 Normals[Map::MapDimensionSize * Map::MapDimensionSize];

			/** @brief Number of entities in the following block
			 */
			size_t EntityCount;
		};

		/** @brief 3D mesh of the map
		 */
		Mesh3D mapMesh;

		/** @brief List of tile heights for each tile
		 */
		jo_fixed tileHeights[Map::MapDimensionSize * Map::MapDimensionSize];

		/** @brief Get vertex index from location
		 * @param x X location
		 * @param y Y location
		 * @return Vertex index
		 */
		constexpr static size_t GetVertexIndex(const size_t& x, const size_t& y)
		{
			return x + (y * (Map::MapDimensionSize + 1));
		}

		/** @brief Get the Tile vertex heights
		 * @param data Level data
		 * @param x Tile X coordinate
		 * @param y Tile Y coordinate
		 * @param result array of 4 depths
		 */
		static void GetTileHeights(const LevelData* data, const int x, const int y, int* result);

	public:

		/** @brief Entity types map will try to spawn
		 */
		enum class EntityType : unsigned int
		{
			/** @brief Empty entity, does nothing
			 */
			Empty = 0,

			/** @brief Player spawn location and direction
			 */
			PlayerSpawn,

			/** @brief 3D model
			 */
			Model,

			/** @brief Item crate
			 */
			Crate
		};

		/** @brief Defines and entity to be created on map
		 */
		struct EntityCreationDefinition
		{
			/** @brief Type of the entity to be created
			 */
			EntityType Type;

			/** @brief Location of the entity in 3D space
			 */
			Vec3 Location;

			/** @brief Angle of the entity in radians
			 */
			Fxp Angle;
			
			/** @brief Reserved space
			 */
			unsigned char Reserved[2];
		};

		/** @brief Level sun light definition
		 */
		struct Sun
		{
			/** @brief Light direction
			 */
			Vec3 Direction;

			/** @brief Light color
			 */
			jo_color Color;
		};

		int EntityDefinitionsCount;

		/** @brief Entities to be created on the map
		 */
		EntityCreationDefinition* EntityDefinitions;

		/** @brief Light definition
		 */
		Sun Light;

		/** @brief Get tile index from location
		 * @param x X location
		 * @param y Y location
		 * @return Tile index
		 */
		constexpr static size_t GetTileIndex(const size_t& x, const size_t& y)
		{
			return x + (y * Map::MapDimensionSize);
		}

		Map(const char* file, int firstTerrainTexture);

		/** @brief Frees all resources and destroys the isntance
		 */
		~Map();

		/** @brief Draw map
		 */
		void Draw();

		/** @brief Get information about specific tile
		 * @param x Tile X location
		 * @param y Tile Y location
		 * @param normal Ground plane normal
		 * @param height Ground height
		 * @param material Ground material
		 * @param collider Tile collider
		 * @param Used tile index
		 */
		int GetTile(int x, int y, Vec3* normal, Fxp* height, uint16_t* material)
		{
			int index = Map::GetTileIndex(x, y);
			*height = Fxp::BuildRaw(this->tileHeights[index]);
			*material = this->mapMesh.attbl[index].texno;

			*normal = Vec3(
				Fxp::BuildRaw(this->mapMesh.pltbl[index].norm[X]),
				Fxp::BuildRaw(this->mapMesh.pltbl[index].norm[Y]),
				Fxp::BuildRaw(this->mapMesh.pltbl[index].norm[Z]));

			return index;
		}
	};

	/** @brief Initializes a new instance of the Map class
	 * @param file Map file name
	 * @param firstTerrainTexture Index of first terrain texture
	 */
	Map::Map(const char* file, int firstTerrainTexture)
	{
		// Load level data
		char* stream = jo_fs_read_file_in_dir(file, JO_ROOT_DIR, NULL);
		char* streamStart = stream;
		LevelData* level = GetAndIterate<LevelData>(stream);

		// Initialize mesh
		this->mapMesh = Mesh3D((Map::MapDimensionSize + 1) * (Map::MapDimensionSize + 1), Map::MapDimensionSize * Map::MapDimensionSize);

		this->Light.Direction = level->Sun.Direction;
		this->Light.Color = level->Sun.Color;

		Vec3 vector = -Light.Direction;
		slLight((FIXED*)&vector);

		// Load gouraud
		for (uint32_t color = 0; color < this->mapMesh.nbPolygon; color++)
		{
			jo_color* ptr = (jo_color*)(JO_VDP1_VRAM + 0x70000 + JO_MULT_BY_8(color));
			*ptr = level->Gouraud[color].Colors[2];
			*(ptr + 1) = level->Gouraud[color].Colors[1];
			*(ptr + 2) = level->Gouraud[color].Colors[0];
			*(ptr + 3) = level->Gouraud[color].Colors[3];
		}

		// Reset point data
		for (size_t x = 0; x <= Map::MapDimensionSize; x++)
		{
			for (size_t y = 0; y <= Map::MapDimensionSize; y++)
			{
				int index = Map::GetVertexIndex(x, y);
				this->mapMesh.pntbl[index][X] = ((x) << 19);
				this->mapMesh.pntbl[index][Y] = ((y) << 19);
				this->mapMesh.pntbl[index][Z] = 0;
			}
		}

		// Load tile geometry
		for (size_t tileY = 0; tileY < Map::MapDimensionSize; tileY++)
		{
			for (size_t tileX = 0; tileX < Map::MapDimensionSize; tileX++)
			{
				// Get tile depths
				int depths[4];
				Map::GetTileHeights(level, tileX, tileY, depths);

				// Get tile location in array
				size_t currentTile = Map::GetTileIndex(tileX, tileY);

				// // Set polygon
				(Vec3&)mapMesh.pltbl[currentTile].norm = level->Normals[currentTile];

				// Set vertex indicies
				size_t vertices[4] = {
					Map::GetVertexIndex(tileX + 1, tileY),
					Map::GetVertexIndex(tileX + 1, tileY + 1),
					Map::GetVertexIndex(tileX, tileY + 1),
					Map::GetVertexIndex(tileX, tileY),
				};

				// Calculate depth
				int depthsRotated[4] = {
					depths[2],
					depths[3],
					depths[1],
					depths[0]
				};

				// Average the 4 vertex depths to get real smooth depth
				int depth = (depths[0] + depths[1] + depths[2] + depths[3]) / 4;
				this->tileHeights[currentTile] = depth;

				// TODO: support tile rotation
				int baseIndex = 3 - level->TileData[currentTile].Rotation;

				for (size_t vertex = 0; vertex < 4; vertex++)
				{
					if (baseIndex >= 4)
					{
						baseIndex = 0;
					}

					this->mapMesh.pntbl[vertices[vertex]][Z] = depthsRotated[vertex];
					this->mapMesh.pltbl[currentTile].Vertices[baseIndex] = vertices[vertex];
					baseIndex++;
				}

				// Set attribute
				ATTR attribute = ATTRIBUTE(
					Dual_Plane,
					SORT_MAX,
					(uint16_t)(firstTerrainTexture + level->TileData[currentTile].Texture),
					JO_COLOR_White,
					CL32KRGB | No_Gouraud,
					CL32KRGB | MESHoff,
					sprHVflip,
					No_Option);

				attribute.gstb = 0xe000 + currentTile;
				JO_ADD_FLAG(attribute.atrb, CL_Gouraud);
				this->mapMesh.attbl[currentTile] = attribute;
			}
		}

		// Load entities to spawn
		EntityDefinitionsCount = level->EntityCount;
		this->EntityDefinitions = new EntityCreationDefinition[level->EntityCount];
		for (size_t entity = 0; entity < level->EntityCount; entity++)
		{
			EntityDefinition* entityPtr = GetAndIterate<EntityDefinition>(stream);
			jo_fixed depth = this->tileHeights[Map::GetTileIndex(entityPtr->TileX, entityPtr->TileY)];

			this->EntityDefinitions[entity] = 
				EntityCreationDefinition
				(
					(EntityType)entityPtr->Type,
					Vec3(
						(Fxp::FromInt(entityPtr->TileX) + 0.5) << 3,
						(Fxp::FromInt(entityPtr->TileY) + 0.5) << 3,
						Fxp::BuildRaw(depth)
					),
					entityPtr->Direction,
					{ entityPtr->Dummy[0], entityPtr->Dummy[1] }
				);
		}

		jo_free(streamStart);
	}

	/** @brief Frees all resources and destroys the isntance
	 */
	Map::~Map() {}

	/** @brief Draw map
	 */
	void Map::Draw()
	{
		jo_3d_mesh_draw(this->mapMesh.JoPtr());
	}


	/** @brief Get the Tile vertex heights
	 * @param data Level data
	 * @param x Tile X coordinate
	 * @param y Tile Y coordinate
	 * @param result
	 */
	void Map::GetTileHeights(const LevelData* data, const int x, const int y, int* result)
	{
		int dim = Map::MapDimensionSize - 1;

		for (size_t py = 0; py < 2; py++)
		{
			int py1 = y + py - 1;
			int py1Min = JO_MIN(py1, dim);
			int tileY1 = JO_MAX(py1Min, 0);

			int py2 = py1 + 1;
			int py2Min = JO_MIN(py2Min, dim);
			int tileY2 = JO_MAX(py2Min, 0);

			for (size_t px = 0; px < 2; px++)
			{
				int px1 = x + px - 1;
				int px1Min = JO_MIN(px1, dim);
				int tileX1 = JO_MAX(px1Min, 0);

				int px2 = tileX1 + 1;
				int px2Min = JO_MIN(px2, dim);
				int tileX2 = JO_MAX(px2Min, 0);

				result[0] = ((int32_t)data->TileData[Map::GetTileIndex(tileX1, tileY1)].Depth << 14);
				result[1] = ((int32_t)data->TileData[Map::GetTileIndex(tileX1, tileY2)].Depth << 14);
				result[2] = ((int32_t)data->TileData[Map::GetTileIndex(tileX2, tileY2)].Depth << 14);
				result[3] = ((int32_t)data->TileData[Map::GetTileIndex(tileX2, tileY1)].Depth << 14);
			}
		}
	}
}
