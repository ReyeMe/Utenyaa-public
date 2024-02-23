#pragma once

#include <stdint-gcc.h>
#include "..\Utils\Message.hpp"

namespace Messages
{
	/** @brief This message is used to querry for controller index
	 */
	struct QueryController : public MessageType<Messages::QueryController>
	{
		/** @brief Controller index
		 */
		uint8_t Controller;

		/** @brief Was the message handled
		 */
		bool Handled;

		/** @brief Construct a new querry message
		 * @param How much damage to cause (negative values can be used to heal) 
		 */
		QueryController() : Handled(false), Controller(0) { }
	};
	
}
