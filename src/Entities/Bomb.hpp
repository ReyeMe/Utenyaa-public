#pragma once

#include <jo/Jo.hpp>

#include "..\Objects\Map.hpp"

#include "..\Utils\Geometry\AABB.hpp"
#include "..\Interfaces\IRenderable.hpp"
#include "..\Interfaces\IUpdatable.hpp"
#include "..\Objects\Terrain.hpp"

#include "..\Messages\Damage.hpp"
#include "..\Messages\QueryController.hpp"

#include "..\Utils\Math\Trigonometry.hpp"

// Managers
#include "..\utils\ModelManager.hpp"

namespace Entities
{
	/** @brief Crate entity
	 */
	struct Bomb : public IRenderable, IUpdatable, TrackableObject<Entities::Bomb>
	{
	private:
		/** @brief How much damage does this object cause
		 */
		inline const static int Damage = 3;

		/** @brief How much damage does this object cause
		 */
		inline const static Fxp BlastRadius = 13.0;

		/** @brief Gravity vecotor
		 */
		inline const static Vec3 Gravity = Vec3(0.0, 0.0, -0.6);

		/** @brief Bullet position
		 */
		Vec3 position;

		/** @brief Bomb velocity
		 */
		Vec3 velocity;

		/** @brief Number of frames left before the mine detonates on its own
		 */
		Fxp timeToDetonate = Fxp::FromInt(3);

		/** @brief Number of frames left before bomb can hurt player who put it there
		 */
		unsigned long safeFrames = 180;

		/** @brief Bomb mesh
		 */
		Objects::Model* mesh;

		/** @brief pulsing animation
		 */
		Fxp pulse;
	public:

		/** @brief Initializes a new instance of the Bomb class
		 * @param originPlayer Player who trew the bomb
		 * @param throwDirection Which direction the bomb will be thrown
		 * @param position Position the bomb will start at
		 */
		Bomb(Vec3& throwDirection, Vec3& position) : position(position), velocity(throwDirection)
		{
			this->velocity.z = Fxp::BuildRaw(delta_time) * 15.0;
			this->mesh = ModelManager::GetModel(6);
		}

		/** @brief Update bullet
		 */
		void Update() override
		{
			Objects::Terrain::Ground ground;
			Objects::Terrain::GetGround(this->position, &ground);
			
			if (ground.Height >= this->position.z)
			{
				this->position.z = ground.Height;
				this->timeToDetonate -= Fxp::BuildRaw(delta_time);

				if (this->timeToDetonate <= 0.0)
				{
					for (auto* object : IColliding::objects)
					{
						if (object != nullptr)
						{
							AABB box;
							object->GetBounds(&box);
	
							if ((box.GetCenter() - this->position).Length() <= Bomb::BlastRadius)
							{
								object->HandleMessages(Messages::Damage(Bomb::Damage));
							}
						}
					}

					PoneSound::Sound::Play(0, PoneSound::PlayMode::Semi, 5);

					Vec3 offset = this->position + Vec3(0.0, 0.0, 3.0);
					new Explosion(offset, 1.0);
					delete this;
					return;
				}
			}
			else
			{
				Fxp boundry = Fxp::BuildRaw(Objects::Map::MapDimensionSize << 19);

				if ((this->position.x + this->velocity.x < 0.0) ||
					(this->position.x + this->velocity.x > boundry))
				{
					this->velocity.x = -(this->velocity.x >> 1);
					this->velocity.z = 0.0;
				}

				if ((this->position.y + this->velocity.y < 0.0) ||
					(this->position.y + this->velocity.y > boundry))
				{
					this->velocity.y = -(this->velocity.y >> 1);
					this->velocity.z = 0.0;
				}

				this->velocity += Bomb::Gravity * Fxp::BuildRaw(delta_time);
				this->position += this->velocity;
			}

			this->pulse += 0.01;

			if (this->pulse > 1.0)
			{
				this->pulse = 0.0;
			}
		}

		/** @brief Draw bullet on screen
		 */
		void Draw() override
		{
			Fxp scale = ((Trigonometry::Sin(this->pulse) >> 1) + 1.5) >> 1;

			jo_3d_push_matrix();
			jo_3d_translate_matrix_fixed(this->position.x.Value(), this->position.y.Value(), (this->position.z + 2.0).Value());
			jo_3d_set_scale_fixed(scale.Value(), scale.Value(), scale.Value());
			this->mesh->Draw();
			jo_3d_pop_matrix();
		}
	};
}