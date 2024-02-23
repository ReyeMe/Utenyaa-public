#pragma once

#include <jo/Jo.hpp>
#include <stdint-gcc.h>
#include "..\utils\LoaderUtil.hpp"

class PakTextureLoader
{
private:
	/** @brief Texture, textures are always RGB1555
	 */
	struct Texture
	{
		/** @brief Width of the texture
		 */
		uint16_t Width;

		/** @brief Height of the texture
		 */
		uint16_t Height;

		size_t LoadSize() const
		{
			return sizeof(Texture) + (sizeof(jo_color) * (Width * Height));
		}

		jo_color* Data() const
		{
			return (jo_color*)(((char*)this) + sizeof(Texture));
		}
	};


public:

	/** @brief Load all textures from pak file
	 * @param file File name
	 * @return int Index of first texture
	 */
	static int LoadTextures(const char * file)
	{
		int length;
		int location = 0;
		char* fileBuffer = jo_fs_read_file_in_dir(file, JO_ROOT_DIR, &length);
		char* iterator = fileBuffer;
		int firstTexture = -1;

		while (location < length)
		{
			// Get header
			Texture* texture = GetAndIterate<Texture>(iterator);
			location += texture->LoadSize();

			// Get texture data
			jo_img img = { texture->Width, texture->Height, texture->Data() };
			int32_t spriteIndex = jo_sprite_add(&img);

			if (firstTexture< 0)
			{
				firstTexture = spriteIndex;
			}
		}
		
		jo_free(fileBuffer);
		return firstTexture;
	}
};
