#pragma once

#include <jo/Jo.hpp>

#include "..\Objects\Map.hpp"

#include "..\Utils\Geometry\AABB.hpp"
#include "..\Interfaces\IRenderable.hpp"
#include "..\Interfaces\IUpdatable.hpp"
#include "..\Objects\Terrain.hpp"

#include "..\Messages\Damage.hpp"
#include "..\Messages\QueryController.hpp"

#include "Explosion.hpp"

namespace Entities
{
	/** @brief Simple bullet entity
	 */
	struct Bullet : IRenderable, IUpdatable, TrackableObject<Entities::Bullet>
	{
	protected:
		/** @brief How much damage does this object cause
		 */
		inline const static int Damage = 2;

		/** @brief Bullet speed
		 */
		inline const static Fxp Speed = 50.0;

		/** @brief Maximal ammout of force dragging bullet down before it is registered ad hard hit
		 */
		inline const static Fxp DownForceLimit = 0.15;

		/** @brief How far above the ground bullet can be
		 */
		inline const static Fxp GroundClearance = 3.0;

		/** @brief ID of the bullet texture
		 */
		inline static uint16_t texture;

		/** @brief Gravity vecotor
		 */
		const Vec3 gravity = Vec3(0.0, 0.0, -10.6);

		/** @brief How long will this entity live
		 */
		uint16_t lifeTime = 0x03ff;

		/** @brief Origin player of the bullet
		 */
		uint8_t origin;

		/** @brief Bullet position
		 */
		Vec3 position;

		/** @brief Bullet velocity 
		 */
		Vec3 velocity;

	public:
		/** @brief Set the Bullet Texture ID
		 * @param Texture ID
		 */
		static void SetTextureId(uint16_t id)
		{
			Bullet::texture = id;
		}

		/** @brief Initializes a new instance of the Bullet class
		 * @param originPlayer Player who shot the bullet
		 * @param direction Direction the bullet will fly in
		 * @param position Position the bullet will start at
		 */
		Bullet(uint8_t originPlayer, Vec3& direction, Vec3& position) : position(position), velocity(direction), origin(originPlayer)
		{
			this->velocity = this->velocity * Bullet::Speed;
			this->position.z += Bullet::GroundClearance;
		}
		
		/** @brief Destroy the Bullet
		 */
		~Bullet() { }
		
		/** @brief Draw bullet on screen
		 */
		void Draw() override
		{
			MATRIX matrix;

			jo_3d_push_matrix();
			jo_3d_translate_matrix_fixed(this->position.x.Value(), this->position.y.Value(), this->position.z.Value());
			jo_3d_set_scale_fixed(Fxp(0.2).Value(), Fxp(0.2).Value(), Fxp(0.2).Value());
			Helpers::DrawSprite(Bullet::texture);
			jo_3d_pop_matrix();
		}

		/** @brief Update bullet
		 */
		void Update() override
		{
			bool destroyBullet = this->lifeTime == 0;

			if (!destroyBullet)
			{
				this->lifeTime--;
				
				// Update position
				Vec3 deltaGravity = Bullet::gravity * Fxp::BuildRaw(delta_time);
				this->velocity += deltaGravity;
				this->position += this->velocity * Fxp::BuildRaw(delta_time);

				// Check against terrain
				Objects::Terrain::Ground ground;
				Objects::Terrain::GetGround(this->position, &ground);
				Fxp groundHeight = ground.Height + Bullet::GroundClearance;

				// Keep the bullet bit above ground, this will also make it roll up small slopes
				if ((this->position + deltaGravity).z >= groundHeight && this->position.z < groundHeight)
				{
					this->velocity -= deltaGravity * Fxp::BuildRaw(delta_time);
					this->position.z = groundHeight;
				}
				else if (this->position.z < groundHeight)
				{
					this->velocity.z = groundHeight - this->position.z;

					if (this->velocity.z > Bullet::DownForceLimit << 1)
					{
						this->velocity = this->velocity * 0.7;
						this->velocity.z = Bullet::DownForceLimit << 5;
					}

					this->position.z = groundHeight;
				}

					// Check if bullet is already too low or collided with something
					destroyBullet = ground.Height > this->position.z || Objects::Terrain::FindCollision(this->position, 0, this->position) != nullptr;

				// Check against dynamic stuff
				IColliding* collidesWith = TrackableObject<IColliding>::FirstOrDefault([this](IColliding* item) { return item->Collide(&this->position); });

				if (collidesWith != nullptr)
				{
					// Check if we are not hitting our selves
					Messages::QueryController query;
					collidesWith->HandleMessages(query);

					if (query.Handled && query.Controller != this->origin)
					{
						collidesWith->HandleMessages(Messages::Damage(Bullet::Damage));
						destroyBullet = true;
					}
				}
			}

			// Delete bullet if it hits something or is out of bounds
			if (destroyBullet ||
				this->position.x >> 19 < 0.0 || this->position.y >> 19 < 0.0 ||
				(this->position.x >> 19).Value() >= Objects::Map::MapDimensionSize || (this->position.y >> 19).Value() >= Objects::Map::MapDimensionSize)
			{
				new Explosion(this->position, 0.25);
				delete this;
			}
		}
	};
}
