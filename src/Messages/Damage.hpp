#pragma once

#include <stdint-gcc.h>
#include "..\Utils\Message.hpp"

namespace Messages
{
	/** @brief Damage taken message
	 */
	struct Damage : public MessageType<Messages::Damage>
	{
		/** @brief How much damage to take
		 */
		int16_t DamageValue;

		/** @brief Construct a new Damage message
		 * @param How much damage to cause (negative values can be used to heal) 
		 */
		Damage(int16_t damage) : DamageValue(damage) { }
	};
	
}
