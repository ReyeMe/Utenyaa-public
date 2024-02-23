#pragma once

#include "..\utils\Math\Vec3.hpp"
#include "Map.hpp"
#include "..\Utils\Geometry\AABB.hpp"
#include "..\Utils\Debug.hpp"

namespace Objects
{
	/**
	 * @brief Interface for terrain object.
	 * 
	 * This interface extends TrackableObject to allow tracking of objects that implement it.
	 */
	struct Terrain
	{
	private:
		/** @brief List of colliders within each tile
		 */
		inline static AABB* tileColliders[Objects::Map::MapDimensionSize * Objects::Map::MapDimensionSize] { nullptr };

	public:
		/** @brief Ground data
		 */
		struct Ground
		{
			/** @brief Height of the ground
			 */
			Fxp Height;

			/** @brief Normal vector
			 */
			Vec3 Normal;

			/** @brief material of the ground
			 */
			uint16_t Material;

			/** @brief Static collider present on this tile
			 */
			AABB* Collider;
		};

		/** @brief Map reference
		 */
		inline static Objects::Map * Map;

		/** @brief Index of first ground texture
		 */
		inline static int FirstGroundTextureIndex = 0;

		/** @brief Clear all tile colliders
		 */
		inline static void InitColliders()
		{
			for (int i = 0; i < Objects::Map::MapDimensionSize * Objects::Map::MapDimensionSize; i++)
			{
				Terrain::tileColliders[i] = nullptr;
			}
		}

		/** @brief Clear all tile colliders
		 */
		inline static void ClearColliders()
		{
			for (int i = 0; i < Objects::Map::MapDimensionSize * Objects::Map::MapDimensionSize; i++)
			{
				if (Terrain::tileColliders[i] != nullptr)
				{
					delete Terrain::tileColliders[i];
					Terrain::tileColliders[i] = nullptr;
				}
			}
		}

		/** @brief Set collision box for ground tile
		 * @param location Tile location
		 * @param collider Collider box
		 */
		inline static void SetGroundCollider(const Vec3& location, AABB* collider)
		{
			// Get tile
			int tileX = location.x.Value() >> 19;
			int tileY = location.y.Value() >> 19;
			int index = Objects::Map::GetTileIndex(tileX, tileY);

			if (Terrain::tileColliders[index] != nullptr)
			{
				delete Terrain::tileColliders[index];
			}

			if (collider != nullptr)
			{
				Terrain::tileColliders[index] = collider;
			}
		}

		/** @brief Get terrain ground
		 * @param result Ground data
		 */
		inline static void GetGround(const Vec3& location, Ground* result)
		{
			int tileIndexSize = Objects::Map::MapDimensionSize - 1;

			// Get tile
			int tileX = location.x.Value() >> 19;
			int tileY = location.y.Value() >> 19;

			// Clamp
			tileX = JO_MAX(tileX, 0);
			tileX = JO_MIN(tileX, tileIndexSize);
			tileY = JO_MAX(tileY, 0);
			tileY = JO_MIN(tileY, tileIndexSize);

			// Get current tile
			int index = Terrain::Map->GetTile(tileX, tileY, &result->Normal, &result->Height, &result->Material);
			result->Material -= Terrain::FirstGroundTextureIndex;
			result->Collider = Terrain::tileColliders[index];
		}

		/** @brief Find first tile collision within range
		 * @param location Center tile location
		 * @param radius Radius in number of tiles (1, would bean area 3x3)
		 * @param collider Collider to check against
		 * @return Found collider of the tile
		 */
		inline static AABB* FindCollision(const Vec3& location, uint16_t radius, IColliding* collider)
		{
			AABB box;
			collider->GetBounds(&box);

			// Get tile
			int tileX = location.x.Value() >> 19;
			int tileY = location.y.Value() >> 19;

			for (int checkX = tileX - radius; checkX <= tileX + radius; checkX++)
			{
				for (int checkY = tileY - radius; checkY <= tileY + radius; checkY++)
				{
					if (checkX >= 0 && checkX < Objects::Map::MapDimensionSize &&
						checkY >= 0 && checkY < Objects::Map::MapDimensionSize)
					{
						int index = Objects::Map::GetTileIndex(checkX, checkY);

						if (Terrain::tileColliders[index] != nullptr && Terrain::tileColliders[index]->Collide(&box))
						{
							return Terrain::tileColliders[index];
						}
					}
				}
			}

			return nullptr;
		}
		
		/** @brief Find first tile collision within range
		 * @param location Center tile location
		 * @param radius Radius in number of tiles (1, would bean area 3x3)
		 * @param collider Point to check against
		 * @return Found collider of the tile
		 */
		inline static AABB* FindCollision(const Vec3& location, uint16_t radius, const Vec3& collider)
		{
			// Get tile
			int tileX = location.x.Value() >> 19;
			int tileY = location.y.Value() >> 19;

			for (int checkX = tileX - radius; checkX <= tileX + radius; checkX++)
			{
				for (int checkY = tileY - radius; checkY <= tileY + radius; checkY++)
				{
					if (checkX >= 0 && checkX < Objects::Map::MapDimensionSize &&
						checkY >= 0 && checkY < Objects::Map::MapDimensionSize)
					{
						int index = Objects::Map::GetTileIndex(checkX, checkY);

						if (Terrain::tileColliders[index] != nullptr && Terrain::tileColliders[index]->Collide(&collider))
						{
							return Terrain::tileColliders[index];
						}
					}
				}
			}

			return nullptr;
		}
	};
}
