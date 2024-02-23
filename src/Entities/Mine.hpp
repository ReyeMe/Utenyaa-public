#pragma once

#include <jo/Jo.hpp>

#include "..\Objects\Map.hpp"

#include "..\Utils\Geometry\AABB.hpp"
#include "..\Interfaces\IRenderable.hpp"
#include "..\Interfaces\IUpdatable.hpp"

#include "..\Messages\Damage.hpp"
#include "..\Messages\QueryController.hpp"
#include "..\Utils\Helpers.hpp"

namespace Entities
{
	/** @brief Crate entity
	 */
	struct Mine : public IRenderable, IUpdatable, TrackableObject<Entities::Mine>
	{
	private:
		/** @brief How much damage does this object cause
		 */
		inline const static int Damage = 2;

		/** @brief ID of the texture
		 */
		inline static uint16_t texture;

		/** @brief Collider size
		 */
		const Fxp Size = 2.0;

		/** @brief Box collider
		 */
		AABB collider;

		/** @brief Origin player of the bullet
		 */
		uint8_t origin;

		/** @brief Bullet position
		 */
		Vec3 position;

		/** @brief Number of frames left before the mine detonates on its own
		 */
		Fxp timeToDetonate = Fxp::FromInt(35);

		/** @brief Number of frames left before bomb can hurt player who put it there
		 */
		Fxp safeTime = Fxp::FromInt(3);
	public:

		/** @brief Set the Bullet Texture ID
		 * @param Texture ID
		 */
		static void SetTextureId(uint16_t id)
		{
			Mine::texture = id;
		}

		/** @brief Initializes a new instance of the Bullet class
		 * @param originPlayer Player who shot the bullet
		 * @param direction Direction the bullet will fly in
		 * @param position Position the bullet will start at
		 */
		Mine(uint8_t originPlayer, Vec3& position) : position(position), origin(originPlayer)
		{
			this->collider = AABB(
				this->position,
				Vec3(Mine::Size, Mine::Size, Mine::Size));
		}

		/** @brief Update bullet
		 */
		void Update() override
		{
			bool destroySelf = false;
			IColliding* collidesWith = TrackableObject<IColliding>::FirstOrDefault([this](IColliding* item) { return item->Collide(&this->collider); });

			if (collidesWith != nullptr)
			{				
				// Check if we are not hitting our selves
				Messages::QueryController query;
				collidesWith->HandleMessages(query);

				if (query.Handled && (query.Controller != this->origin || this->safeTime <= 0.0))
				{
					collidesWith->HandleMessages(Messages::Damage(Mine::Damage));
					destroySelf = true;
				}
			}

			if (destroySelf || this->timeToDetonate <= 0.0)
			{
				Vec3 pos = this->position + Vec3(0.0, 0.0, 1.0);
				new Explosion(pos, 0.5);
				PoneSound::Sound::Play(0, PoneSound::PlayMode::Semi, 5);
				delete this;
			}
			else
			{
				if (this->safeTime >= 0.0)
				{
					this->safeTime -= Fxp::BuildRaw(delta_time);
				}

				this->timeToDetonate-= Fxp::BuildRaw(delta_time);
			}
		}

		/** @brief Draw bullet on screen
		 */
		void Draw() override
		{
			MATRIX matrix;

			jo_3d_push_matrix();
			jo_3d_translate_matrix_fixed(this->position.x.Value(), this->position.y.Value(), (this->position.z + 1.0).Value());
			jo_3d_set_scale_fixed(Fxp(0.2).Value(), Fxp(0.2).Value(), Fxp(0.2).Value());
			Helpers::DrawSprite(Mine::texture);
			jo_3d_pop_matrix();
		}
	};
}