#pragma once

#include <jo/Jo.hpp>

#include "..\Utils\Math\Vec3.hpp"
#include "..\Interfaces\IRenderable.hpp"
#include "..\Interfaces\IUpdatable.hpp"
#include "..\Utils\Helpers.hpp"

namespace Entities
{
	/** @brief Explosion particle
	 */
	struct Explosion : public IRenderable, IUpdatable, TrackableObject<Entities::Explosion>
	{
	private:

		/** @brief Time in seconds between frames
		 */
		inline const static Fxp FrameTime = 0.1;

		/** @brief Total number of frames
		 */
		inline const static unsigned char TotalFrames = 6;

		/** @brief Explosion sprite
		 */
		inline static int texture;

		/** @brief Sprite position
		 */
		Vec3 position;

		/** @brief Sprite scale
		 */
		Fxp scale;

		/** @brief Current frame
		 */
		unsigned char frame;

		/** @brief Currently passed time
		 */
		Fxp passedTime;

	public:
		/** @brief Set the explosion Texture ID
		 * @param Texture ID
		 */
		static void SetTextureId(uint16_t id)
		{
			Explosion::texture = id;
		}

		/** @brief Initializes a new instance of the Bomb class
		 * @param position Where to spawn explosion
		 * @param scale Sprite scale
		 */
		Explosion(Vec3& position, Fxp scale) : position(position), scale(scale)
		{
			this->frame = 0;
			this->passedTime = 0.0;
		}

		/** @brief Update bullet
		 */
		void Update() override
		{
			if (Explosion::FrameTime <= this->passedTime)
			{
				this->frame++;
				this->passedTime = 0.0;
			}

			if (Explosion::TotalFrames == this->frame)
			{
				delete this;
				return;
			}

			this->passedTime += Fxp::BuildRaw(delta_time);
		}
		
		/** @brief Draw bullet on screen
		 */
		void Draw() override
		{
			jo_3d_push_matrix();
			jo_3d_translate_matrix_fixed(this->position.x.Value(), this->position.y.Value(), this->position.z.Value());
			jo_3d_set_scale_fixed(this->scale.Value(), this->scale.Value(), this->scale.Value());
			
			Helpers::DrawSprite(Explosion::texture + this->frame);

			jo_3d_pop_matrix();
		}
	};
	
}
