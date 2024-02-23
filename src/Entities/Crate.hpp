#pragma once

#include <jo/Jo.hpp>

#include "..\Utils\ponesound\ponesound.hpp"
#include "Player.hpp"

namespace Entities
{
	/** @brief Crate entity
	 */
	struct Crate : public IRenderable, IUpdatable, TrackableObject<Entities::Crate>
	{
	private:
		/** @brief How much does health pack give
		 */
		const short HealthPack = 2;

		/** @brief Movement speed in units per frame
		 */
		const Fxp FallSpeed = 15.0;

		/** @brief How high should the box spawn
		 */
		const Fxp SpawnHeight = 150.0;

		/** @brief Collider size
		 */
		const Fxp Size = 4.0;

		/** @brief Position of the detail in the world
		 */
		Vec3 position;

		/** @brief Time left before spawning 
		 */
		Fxp timeToSpawn;

		/** @brief Respawn time
		 */
		Fxp respawnTime;

		/** @brief Crate model
		 */
		Objects::Model * model;

		/** @brief Has crate landed
		 */
		bool isOnGround;

		/** @brief Height of the ground
		 */
		Fxp groundHeight;

		/** @brief Box collider
		 */
		AABB collider;

		/** @brief Crate rotation
		 */
		ANGLE rotation;

		/** @brief Can spawn health pack
		 */
		bool canSpawnHealth;

		/** @brief Can spawn bomb
		 */
		bool canSpawnBomb;

		/** @brief Can spawn mine
		 */
		bool canSpawnMine;

		/** @brief Oscillating movement when falling
		 */
		Fxp oscillation;
		
		/** @brief Oscillating movement step when falling
		 */
		Fxp OscillationStep = 0.007;

	public:

		/** @brief Initializes a new instance of the Crate class
		 * @param position Spawner position
		 * @param flags Spawn flags
		 * @param respawnTime respawn time
		 */
		Crate(const Vec3& position, const unsigned char flags, const unsigned char respawnTime) : position(position)
		{
			this->canSpawnHealth = flags & 0x01;
			this->canSpawnBomb = flags & 0x02;
			this->canSpawnMine = flags & 0x04;

			this->respawnTime = Fxp::FromInt(respawnTime);
			this->timeToSpawn = this->respawnTime;
			this->isOnGround = false;
			this->model = ModelManager::GetModel(0);
			this->position.z = Crate::SpawnHeight;
			
			// Get current ground tile
			Objects::Terrain::Ground ground;
			Objects::Terrain::GetGround(this->position, &ground);
			groundHeight = ground.Height;

			this->rotation = Trigonometry::RadiansToSgl(Fxp::FromInt(jo_random(6)));
		}

		/** @brief Make entity think
		 */
		void Update() override
		{
			if (this->isOnGround)
			{
				Player* collidesWith = TrackableObject<Player>::FirstOrDefault([this](Player* item) { return item->Collide(&this->collider); });

				if (collidesWith != nullptr)
				{
					// Apply effect to player
					int random = jo_random(30);

					if (random > 25 && this->canSpawnBomb)
					{
						collidesWith->HandleMessages(Messages::Pickup(Messages::Pickup::PickupType::Bomb));
						random = -1;
					}
					
					if (random > 18 && this->canSpawnHealth)
					{
						collidesWith->HandleMessages(Messages::Damage(-Crate::HealthPack));
						random = -1;
					}

					if (random >= 0 && this->canSpawnMine)
					{
						collidesWith->HandleMessages(Messages::Pickup(Messages::Pickup::PickupType::Mine));
						random = -1;
					}
					
					if (random < 0)
					{
						this->isOnGround = false;
						this->timeToSpawn = this->respawnTime;
						this->position.z = Crate::SpawnHeight;
						this->rotation = Trigonometry::RadiansToSgl(Fxp::FromInt(jo_random(6)));
						PoneSound::Sound::Play(3, PoneSound::PlayMode::Semi, 5);
					}
				}
			}
			else if (this->timeToSpawn <= 0.0)
			{
				if (this->position.z == Crate::SpawnHeight)
				{
					PoneSound::Sound::Play(4, PoneSound::PlayMode::Semi, 5);
				}

				this->position.z -= Crate::FallSpeed * Fxp::BuildRaw(delta_time);

				if (this->position.z < this->groundHeight)
				{
					this->position.z = this->groundHeight;
					this->isOnGround = true;
					
					// Set collision
					this->collider = AABB(
						this->position,
						Vec3(Crate::Size, Crate::Size, Crate::Size));

					PoneSound::Sound::Play(2, PoneSound::PlayMode::Semi, 5);
				}
			}
			else
			{
				this->timeToSpawn -= Fxp::BuildRaw(delta_time);
			}
		}

		/** @brief Render world
		 */
		void Draw() override
		{
			if (this->timeToSpawn <= 0.0)
			{
				jo_3d_push_matrix();
				jo_3d_translate_matrix_fixed(this->position.x.Value(), this->position.y.Value(), this->position.z.Value());
				slRotZ(this->rotation);

				if (this->isOnGround)
				{
					this->model->Draw(1);
				}
				else
				{
					if (this->oscillation < -0.100 ||
						this->oscillation > 0.100)
					{
						this->OscillationStep = -this->OscillationStep;
					}

					this->oscillation += this->OscillationStep;

					slRotY(Trigonometry::RadiansToSgl(this->oscillation));
					slRotX(-Trigonometry::RadiansToSgl(this->oscillation));
					this->model->Draw();
				}

				jo_3d_pop_matrix();
			}
		}
	};
}
