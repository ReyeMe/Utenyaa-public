#pragma once

#include <jo/Jo.hpp>

#include "..\Interfaces\IRenderable.hpp"
#include "..\Utils\ModelManager.hpp"
#include "..\Utils\Math\Trigonometry.hpp"
#include "..\Objects\Terrain.hpp"

namespace Entities
{
	/** @brief Static 3D detail
	 */
	struct StaticDetail3D : public IRenderable, TrackableObject<Entities::StaticDetail3D>
	{
	protected:

		/** @brief Position of the detail in the world
		 */
		Vec3 Position;

		/** @brief Model of the detail
		 */
		unsigned short model;

		/** @brief SGL angle
		 */
		ANGLE Angle;

	public:

		/** @brief Initializes a new instance of the StaticDetail3D class
		 * @param position Position in the scene
		 * @param angle Entity rotation
		 * @param model render model of the static detail
		 */
		StaticDetail3D(const Vec3& position, Fxp angle, unsigned short model) : Position(position), model(model)
		{ 
			this->Angle = Trigonometry::RadiansToSgl(angle);
			static Fxp size = 3.0;

			// Get current ground tile
			Objects::Terrain::SetGroundCollider(this->Position, new AABB(this->Position, Vec3(size, size, size << 2)));
		}

		/** @brief Draw detail
		 */
		void Draw() override
		{
			jo_3d_push_matrix();
			jo_3d_translate_matrix_fixed(this->Position.x.Value(), this->Position.y.Value(), this->Position.z.Value());
			slRotZ(this->Angle);
			ModelManager::GetModel(this->model)->Draw();
			jo_3d_pop_matrix();
		}
	};
}
