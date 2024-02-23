#pragma once

#include <jo/Jo.hpp>

struct Helpers
{
private:
	/** @brief All our sprites
	 */
	inline static Objects::Mesh3D* Sprites[UTE_MAX_SPRITE] = { nullptr };

public:

	/** @brief Get nth available controller
	 * @param index Controller index
	 * @return int Available controller port
	 */
	inline static int GetNthAvailableController(int index)
	{
		int available = 0;

		for (int controller = 0; controller < JO_INPUT_MAX_DEVICE; controller++)
		{
			if (jo_is_input_available(controller))
			{
				if (available == index)
				{
					return controller;
				}

				available++;
			}
		}

		return -1;
	}

	/** @brief Is controller button pressed
	 * @param controller Controller number
	 * @param key Controller button
	 * @return true if button is pressed
	 */
	inline static bool IsControllerButtonPressed(int controller, jo_gamepad_keys key)
	{
		int input = Helpers::GetNthAvailableController(controller);

		if (input >= 0)
		{
			return jo_is_input_key_pressed(input, key);
		}

		return false;
	}

	/** @brief Is controller button down
	 * @param controller Controller number
	 * @param key Controller button
	 * @return true if button is down
	 */
	inline static bool IsControllerButtonDown(int controller, jo_gamepad_keys key)
	{
		int input = Helpers::GetNthAvailableController(controller);

		if (input >= 0)
		{
			return jo_is_input_key_down(input, key);
		}

		return false;
	}

	/** @brief Draw 3D sprite
	 * @param sprite sprite index
	 */
	inline static void DrawSprite(int sprite)
	{
		// Initialize sprite
		if (Helpers::Sprites[sprite] == nullptr)
		{
    		int img_width = JO_DIV_BY_2(jo_sprite_get_width(sprite));
    		int img_height = JO_DIV_BY_2(jo_sprite_get_height(sprite));

			Helpers::Sprites[sprite] = new Objects::Mesh3D(4, 1);

			// Set plane
			for (int i = 0; i < 4; i++)
			{
				Helpers::Sprites[sprite]->pltbl[0].Vertices[i] = i;
			}

			// Set normal
    		JO_ZERO(Helpers::Sprites[sprite]->pltbl[0].norm[X]);
    		JO_ZERO(Helpers::Sprites[sprite]->pltbl[0].norm[Y]);
			Helpers::Sprites[sprite]->pltbl[0].norm[Z] = JO_FIXED_1;

			// Set vertices
			Helpers::Sprites[sprite]->pntbl[0][X] = JO_MULT_BY_65536(-img_width);
    		Helpers::Sprites[sprite]->pntbl[1][X] = JO_MULT_BY_65536(img_width);
    		Helpers::Sprites[sprite]->pntbl[2][X] = JO_MULT_BY_65536(img_width);
    		Helpers::Sprites[sprite]->pntbl[3][X] = JO_MULT_BY_65536(-img_width);
    		Helpers::Sprites[sprite]->pntbl[0][Y] = JO_MULT_BY_65536(-img_height);
    		Helpers::Sprites[sprite]->pntbl[1][Y] = JO_MULT_BY_65536(-img_height);
    		Helpers::Sprites[sprite]->pntbl[2][Y] = JO_MULT_BY_65536(img_height);
    		Helpers::Sprites[sprite]->pntbl[3][Y] = JO_MULT_BY_65536(img_height);
    		JO_ZERO(Helpers::Sprites[sprite]->pntbl[0][Z]);
    		JO_ZERO(Helpers::Sprites[sprite]->pntbl[1][Z]);
    		JO_ZERO(Helpers::Sprites[sprite]->pntbl[2][Z]);
    		JO_ZERO(Helpers::Sprites[sprite]->pntbl[3][Z]);

			// Set attribute
			Helpers::Sprites[sprite]->attbl->flag = Dual_Plane;
    		jo_3d_set_texture((jo_3d_quad*)Helpers::Sprites[sprite]->JoPtr(), sprite);
    		jo_3d_set_screen_doors((jo_3d_quad*)Helpers::Sprites[sprite]->JoPtr(), false);
    		jo_3d_set_light((jo_3d_quad*)Helpers::Sprites[sprite]->JoPtr(), false);
		}

		// Draw sprite
    	slPutPolygon((PDATA*)Helpers::Sprites[sprite]);
	}

	/** @brief Hide logo layer
	 */
	inline static void HideLogo()
	{
		slScrAutoDisp(NBG1OFF | NBG0ON);
	}

	/** @brief Show logo layer
	 */
	inline static void ShowLogo()
	{
		slScrAutoDisp(NBG1ON | SPRON | NBG0ON);
	}
};

