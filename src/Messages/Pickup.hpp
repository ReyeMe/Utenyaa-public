#pragma once

#include <stdint-gcc.h>
#include "..\Utils\Message.hpp"

namespace Messages
{
	/** @brief Pickup picked up message
	 */
	struct Pickup : public MessageType<Messages::Pickup>
	{
		/** @brief Pickup type
		 */
		enum class PickupType
		{
			/** @brief No pickup
			 */
			None,

			/** @brief Bomb type
			 */
			Bomb,

			/** @brief Mine type
			 */
			Mine
		};

		/** @brief What pickup
		 */
		PickupType Identifier;

		/** @brief Construct a new Pickup message
		 * @param What pickup we got
		 */
		Pickup(PickupType identifier) : Identifier(identifier) { }
	};
	
}
