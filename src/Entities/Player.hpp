#pragma once

#include <jo/Jo.hpp>

#include "..\utils\Geometry\AABB.hpp"
#include "..\utils\Math\Trigonometry.hpp"
#include "..\utils\Math\Plane3D.hpp"

#include "..\Objects\Model.hpp"
#include "..\utils\ModelManager.hpp"
#include "..\utils\UI.hpp"

#include "Bullet.hpp"
#include "Mine.hpp"
#include "Bomb.hpp"

#include "..\Messages\Damage.hpp"
#include "..\Messages\Pickup.hpp"
#include "..\Messages\QueryController.hpp"

#include "..\Utils\Helpers.hpp"

namespace Entities
{
	/** @brief Static 3D detail
	 */
	struct Player : public IRenderable, IUpdatable, IColliding, TrackableObject<Entities::Player>
	{
	private:
		/** @brief Player size
		 */
		const Fxp Size = 4.0;

		/** @brief Number of frames controller has
		 */
		const int FramesPerController = 5;

		/** @brief Rotation speed in radians per frame
		 */
		const Fxp RotationSpeed = 4.0;

		/** @brief Movement speed in units per frame
		 */
		const Fxp MovementSpeed = 30.0;

		/** @brief Maximum health player can have
		 */
		const int16_t MaxHealth = 6;

		/** @brief Index of first character sprite
		 */
		inline static int CharacterSpiteStart;

		/** @brief Direction of the player
		 */
		Fxp angle;

		/** @brief Controller ID
		 */
		uint8_t controller;

		/** @brief Player health
		 */
		int16_t health = Player::MaxHealth;

		/** @brief Model of the detail
		 */
		Objects::Model* model;

		/** @brief Position of the detail in the world
		 */
		Vec3 position;

		/** @brief How much time is left before player can shoot again
		 */
		uint8_t shootCoolDownTimeLeft;

		/** @brief What pickup player has
		 */
		Messages::Pickup::PickupType hasPickup = Messages::Pickup::PickupType::None;

		/** @brief Handle actions like shooting or others
		 */
		void HandleActions()
		{
			ANGLE angle = Trigonometry::RadiansToSgl(this->angle);
			Vec3 movementDir = Vec3(
				Fxp::BuildRaw(slCos(angle)),
				Fxp::BuildRaw(slSin(angle)),
				0.0);

			// Do the shooting
			if (Helpers::IsControllerButtonDown(this->controller, JO_KEY_A) && this->shootCoolDownTimeLeft == 0)
			{
				PoneSound::Sound::Play(1, PoneSound::PlayMode::Semi, 5);
				this->shootCoolDownTimeLeft = 0x1b;
				new Entities::Bullet(this->controller, movementDir, this->position);
			}

			// Use pickup
			if (Helpers::IsControllerButtonDown(this->controller, JO_KEY_B))
			{
				switch (this->hasPickup)
				{
				case Messages::Pickup::PickupType::Mine:
					new Mine(this->controller, this->position);
					break;
				
				case Messages::Pickup::PickupType::Bomb:
					new Bomb(movementDir, this->position);
					break;

				default:
					break;
				}

				// We have used the pickup
				this->hasPickup = Messages::Pickup::PickupType::None;
			}
		}

		/** @brief Handle player movement
		 */
		void HandleMovement()
		{
			Fxp moveBy;
			Fxp rotateBy;

			// Rotate player
			if (Helpers::IsControllerButtonPressed(this->controller, JO_KEY_LEFT))
			{
				rotateBy = Player::RotationSpeed;
			}
			else if (Helpers::IsControllerButtonPressed(this->controller, JO_KEY_RIGHT))
			{
				rotateBy = -Player::RotationSpeed;
			}

			// Move player
			if (Helpers::IsControllerButtonPressed(this->controller, JO_KEY_UP))
			{
				moveBy = Player::MovementSpeed;
			}
			else if (Helpers::IsControllerButtonPressed(this->controller, JO_KEY_DOWN))
			{
				moveBy = -Player::MovementSpeed;
			}

			// Clamp rotation
			if (rotateBy != 0.0)
			{
				this->angle += rotateBy * Fxp::BuildRaw(delta_time);

				if (this->angle < 0.0)
				{
					this->angle += Fxp(Trigonometry::RadPi * 2.0);
				}
				else if (this->angle >= Fxp(Trigonometry::RadPi * 2.0))
				{
					this->angle -= Fxp(Trigonometry::RadPi * 2.0);
				}
			}

			// Try move
			if (moveBy != 0.0)
			{
				// Get current ground tile
				Objects::Terrain::Ground ground;
				ground.Height = 0.0;
				Objects::Terrain::GetGround(this->position, &ground);

				// Waterlogged tiles
				if (ground.Material == 4 || ground.Material == 6)
				{
					moveBy *= 0.6;
				}

				ANGLE angle = Trigonometry::RadiansToSgl(this->angle);
				moveBy = moveBy * Fxp::BuildRaw(delta_time);
				Vec3 movementDir = Vec3(
					Fxp::BuildRaw(slCos(angle)) * moveBy,
					Fxp::BuildRaw(slSin(angle)) * moveBy,
					0.0);

				Fxp boundry = Fxp::BuildRaw(Objects::Map::MapDimensionSize << 19);

				if ((this->position.x + movementDir.x - Player::Size < 0.0) ||
					(this->position.x + movementDir.x + Player::Size > boundry))
				{
					movementDir.x = 0.0;
				}

				if ((this->position.y + movementDir.y - Player::Size < 0.0) ||
					(this->position.y + movementDir.y + Player::Size > boundry))
				{
					movementDir.y = 0.0;
				}

				this->position += movementDir;

				// Get new ground tile
				Objects::Terrain::GetGround(this->position, &ground);

				// Get ground height
				Vec3 location = Vec3(
					((this->position.x >> 3).TruncateFraction() + 0.5) << 3,
					((this->position.y >> 3).TruncateFraction() + 0.5) << 3,
					ground.Height + 1.0);

				Plane3D plane = Plane3D(ground.Normal, location);
				Fxp distance = plane.Distance(Vec3(this->position.x, this->position.y, 0.0));
				movementDir.z = distance - this->position.z;
				this->position.z = distance;

				// Check for collision
				// Find first moving collider, not neccesary the closest
				IColliding* collidesWith = TrackableObject<IColliding>::FirstOrDefault([this](IColliding* item) { return item != this && item->Collide(this); });

				// We have collided with something dynamic, move back
				if (collidesWith != nullptr)
				{
					// Move back
					this->position -= movementDir;

					// Find max axis to limit
					AABB colliderBox;
					collidesWith->GetBounds(&colliderBox);
					Vec3 center = colliderBox.GetCenter();
					Vec3 axies = this->position - center;
					int maxAxis = 0;

					for (int axis = Y; axis < XYZ; axis++)
					{
						if (((Fxp*)&axies)[axis].Abs() > ((Fxp*)&axies)[maxAxis].Abs())
						{
							maxAxis = axis;
						}
					}

					// Limit axis
					((Fxp*)&movementDir)[maxAxis] = 0.0;
				
					// Move player by new vector
					this->position += movementDir >> 1;
				}
				
				// Find static colliders
				AABB* staticCollision = Objects::Terrain::FindCollision(this->position, 2, this);

				// On static collision just move back
				if (staticCollision != nullptr)
				{
					// Do not move player
					this->position -= movementDir;
				}
			}
		}

	public:
		/** @brief Set the texture ID of first character sprite
		 * @param Texture ID
		 */
		static void SetTextureId(uint16_t id)
		{
			Player::CharacterSpiteStart = id;
		}

		/** @brief Initializes a new instance of the StaticDetail3D class
		 * @param position Position in the scene
		 * @param model render model of the static detail
		 * @param controller Controller ID
		 */
		Player(const Vec3& position, Fxp angle, uint8_t controller) : position(position), angle(angle), controller(controller)
		{
			this->shootCoolDownTimeLeft = 0;
			this->model = ModelManager::GetModel(1);
		}

		/** @brief Get the Health
		 */
		int GetHealth()
		{
			return this->health;
		}

		/** @brief Get player controller
		 */
		int GetController()
		{
			return this->controller;
		}

		/** @brief Get object bounds
		 * @param result Axis aligned bounding box
		 */
		void GetBounds(AABB* result) override
		{
			*result = AABB(
				this->position,
				Vec3(Player::Size, Player::Size, Player::Size));
		}

		/** @brief Handle messages sent to this entity
		 * @param message Recieved message
		 */
		void HandleMessages(Message& message) override
		{
			switch (message.typeId)
			{
			case Messages::Damage::Type:
				if (this->health > 0)
				{
					this->health -= ((Messages::Damage*)&message)->DamageValue;
					this->health = JO_MAX(this->health, 0);
					this->health = JO_MIN(this->health, Player::MaxHealth);
				}
				break;
			
			case Messages::QueryController::Type:
				((Messages::QueryController*)&message)->Handled = true;
				((Messages::QueryController*)&message)->Controller = this->controller;
				break;

			case Messages::Pickup::Type:
				this->hasPickup = ((Messages::Pickup*)&message)->Identifier;
				break;

			default:
				break;
			}
		}

		/** @brief Handle messages sent to this entity
		 * @param message Recieved message
		 */
		void HandleMessages(const Message& message) override
		{
			this->HandleMessages(const_cast<Message&>(message));
		}

		/** @brief Indicates whether collider is enabled
		 * @return Returns true
		 */
		bool IsColliderEnabled() override
		{
			return true;
		}

		/** @brief Make entity think
		 */
		void Update() override
		{
			// Handle shoot cool down
			if (this->shootCoolDownTimeLeft > 0)
			{
				this->shootCoolDownTimeLeft--;
			}

			// Allow player to do actions only if alive and controller is connected
			if (this->IsColliderEnabled() && this->health > 0)
			{
				this->HandleMovement();
				this->HandleActions();
			}

			UI::Messages::UpdatePlayer playerUpdate;

			playerUpdate.health = this->health;
			playerUpdate.index = controller;
			playerUpdate.powerupType = (int)this->hasPickup;

			UI::HudHandler.HandleMessages(playerUpdate);
		}

		/** @brief Draw detail
		 */
		void Draw() override
		{
			// Draw body
			jo_3d_push_matrix();
			jo_3d_translate_matrix_fixed(this->position.x.Value(), this->position.y.Value(), (this->position.z + 1.0).Value());
			slRotZ(Trigonometry::RadiansToSgl(this->angle));

			this->model->Draw(1);

			if (this->shootCoolDownTimeLeft > 0x10)
			{
				this->model->Draw(0);
			}

			jo_3d_pop_matrix();

			// Draw head
			jo_3d_push_matrix();
			jo_3d_translate_matrix_fixed(this->position.x.Value(), (this->position.y - 1.0).Value(), (this->position.z + 4.0).Value());
			Fxp mirror = 0.4;

			int index = (this->health > 0) ? this->controller : 4;
			int ang = Trigonometry::RadiansToDeg(this->angle);
			int frame = 0;

			if (ang <= 291 && ang >= 256)
			{
				mirror = -mirror;
				frame = 4;
			}
			else if (ang <= 256 && ang >= 201)
			{
				mirror = -mirror;
				frame = 3;
			}
			else if (ang <= 201 && ang >= 156)
			{
				mirror = -mirror;
				frame = 2;
			}
			else if (ang <= 156 && ang >= 111)
			{
				mirror = -mirror;
				frame = 1;
			}
			else if (ang <= 111 && ang >= 66)
			{
				mirror = -mirror;
				frame = 0;
			}
			else if (ang <= 66 && ang >= 21)
			{
				frame = 1;
			}
			else if (ang <= 336 && ang >= 291)
			{
				frame = 3;
			}
			else if (ang <= 21 || ang >= 336)
			{
				frame = 2;
			}
			
			jo_3d_set_scale_fixed(mirror.Value(), Fxp(0.4).Value(), Fxp(0.4).Value());

			Helpers::DrawSprite(this->CharacterSpiteStart + (index * Player::FramesPerController) + frame);
			
			jo_3d_pop_matrix();
		}
	};
}
