
extern "C" {
#include <jo/jo.h>
#include "pcmsys.h"
}

#include "ponesound.hpp"

/** @brief Initializes ponesound driver
 * @param mode ADX data mode
 */
void PoneSound::Driver::Initialize(const PoneSound::ADXMode mode)
{
	// Load driver
	load_drv((int)mode);
	
    // V-Blank callback for audio handling
    jo_core_add_vblank_callback(sdrv_vblank_rq);

	// set CD audio volume
	CDDA_SetVolume(7);
}

/** @brief Load PCM sound effect
 * @param file File name
 * @param bitDepth Bit depth of the sound effect
 * @param sampleRate Sample rate of the sound effect
 * @return Sound effect identifier (0xffff on fail)
 */
short PoneSound::Sound::LoadPcm(char * file, const PoneSound::PCMBitDepth bitDepth, const int sampleRate)
{
	switch (bitDepth)
	{
	case PoneSound::PCMBitDepth::PCM8:
		return load_8bit_pcm((Sint8 *)file, sampleRate);
	
	case PoneSound::PCMBitDepth::PCM16:
		return load_16bit_pcm((Sint8 *)file, sampleRate);

	default:
		return 0xffff;
	}
}

/** @brief Load ADX sound effect
 * @param file File name
 * @return Sound effect identifier
 */
short PoneSound::Sound::LoadAdx(char * file)
{
	return load_adx((Sint8 *)file);
}

/** @brief Play sound
 * @param sound Sound to play
 * @param mode Loop/Playback mode mode
 * @param volume Starting volume 
 */
void PoneSound::Sound::Play(const short sound, const PoneSound::PlayMode mode, const unsigned char volume)
{
	pcm_play(sound, (char)mode, volume);
}

/** @brief Set master volume
 * @param volume New volume (0-15)
 */
void PoneSound::Driver::SetMasterVolume(const unsigned short volume)
{
	set_master_volume(volume);
}

/** @brief Set volume of currently playing sound
 * @param sound Sound to modify
 * @param volume New volume
 * @param pan Stereo pan to set (right being 0, left being 16)
 */
void PoneSound::Sound::SetVolume(const short sound, const unsigned char volume, const unsigned char pan)
{
	pcm_parameter_change(sound, volume, pan);
}

/** @brief Stop playing sound
 * @param sound Sound to stop
 */
void PoneSound::Sound::Stop(const short sound)
{
	pcm_cease(sound);
}

/** @brief Will remove all sounds after the specified sound
 * @param lastToKeep Index of the last sound to be kept loaded
 */
void PoneSound::Sound::Unload(const short lastToKeep)
{
	pcm_reset(lastToKeep);
}

/** @brief Set CD playback volume
 *  @param volume Audio volume (7 is max)
 */
void PoneSound::CD::SetVolume(const unsigned char volume)
{
	CDDA_SetVolume(volume);
}

/** @brief Set CD playback stereo pan
 *  @param left Left channel volume (7 is max)
 *  @param right Right channel volume (7 is max)
 */
void PoneSound::CD::SetPan(const unsigned char left, const unsigned char right)
{
	CDDA_SetChannelVolPan(left, right);
}

/** @brief Play range of tracks
 *  @param fromTrack Starting track
 *  @param toTrack Ending track
 *  @param loop Whether to play the range of track again after it ends
 */
void PoneSound::CD::Play(const int fromTrack, const int toTrack, const bool loop)
{
    CDDA_Play(fromTrack, toTrack, loop);
}

/** @brief Stop CD music playback
 */
void PoneSound::CD::Stop()
{
    CDDA_Stop();
}
