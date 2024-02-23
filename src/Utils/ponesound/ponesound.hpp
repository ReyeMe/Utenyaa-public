#pragma once

/** @brief Ponuts audio driver
 */
namespace PoneSound
{
	/** @brief ADX data mode
	 */
	enum class ADXMode
	{
		/** @brief 7.68 Data
		 */
		ADX768 = 0,

		/** @brief 11.52 Data
		 */
		ADX1152 = 1,
		
		/** @brief 15.36 Data
		 */
		ADX1536 = 2,
		
		/** @brief 23.04 Data
		 */
		ADX2304 = 3,
	};

	/** @brief Bit depth of PCM sound effects
	 */
	enum class PCMBitDepth
	{
		/** @brief 8-bit
		 */
		PCM8 = 0,
		
		/** @brief 16-bit
		 */
		PCM16 = 1
	};

	/** @brief SOund play mode
	 */
	enum class PlayMode
	{
		/** @brief Sound effect will loop, playback direction will change when playback reaches its end
		 */
		AlternatingLoop = 3,

		/** @brief Sound effect will play backwards in a loop
		 */
		ReversLoop = 2,

		/** @brief Sound effect will play normaly in loop
		 */
		ForwardLoop = 1,

		/** @brief Sound effect will play normaly
		 */
		Volatile = 0,

		/** @brief Sound effect will play normaly, but when stopped, it wil not stop playing until its end
		 */
		Protected = -1,

		/** @brief NO clue what this means
		 */
		Semi = -2,

		/** @brief ADF format sound effect will play
		 */
		ADX = -3
	};

	/** @brief Driver/audio management
	 */
	class Driver
	{
	public:

		/** @brief Initialize sound driver
		 * @param mode ADX data mode
		 */
		static void Initialize(const PoneSound::ADXMode mode);
		
		/** @brief Set master volume
		 * @param volume New volume (0-15)
		 */
		static void SetMasterVolume(const unsigned short volume);
	};
	
	/** @brief Playback of sound effects
	 */
	class Sound
	{
	public:

		/** @brief Load PCM sound effect
		 * @param file File name
		 * @param bitDepth Bit depth of the sound effect
		 * @param sampleRate Sample rate of the sound effect
		 * @return Sound effect identifier
		 */
		static short LoadPcm(char * file, const PoneSound::PCMBitDepth bitDepth, const int sampleRate);

		/** @brief Load ADX sound effect
		 * @param file File name
		 * @return Sound effect identifier
		 */
		static short LoadAdx(char * file);
		
		/** @brief Play sound
		 * @param sound Sound to play
		 * @param mode Loop/Playback mode mode
		 * @param volume Starting volume 
		 */
		static void Play(const short sound, const PoneSound::PlayMode mode, const unsigned char volume);

		/** @brief Stop playing sound
		 * @param sound Sound to stop
		 */
		static void Stop(const short sound);

		/** @brief Set volume of currently playing sound
		 * @param sound Sound to modify
		 * @param volume New volume
		 * @param pan Stereo pan to set (right being 0, left being 16)
		 */
		static void SetVolume(const short sound, const unsigned char volume, const unsigned char pan);
		
		/** @brief Will remove all sounds after the specified sound
		 * @param lastToKeep Index of the last sound to be kept loaded
		 */
		static void Unload(const short lastToKeep);
	};
	
	/** @brief Playback of CD audio
	 */
	class CD
	{
	public:

		/** @brief Set CD playback volume
		 *  @param volume Audio volume (7 is max)
		 */
		static void SetVolume(const unsigned char volume);

		/** @brief Set CD playback stereo pan
		 *  @param left Left channel volume (7 is max)
		 *  @param right Right channel volume (7 is max)
		 */
		static void SetPan(const unsigned char left, const unsigned char right);

		/** @brief Play range of tracks
		 *  @param fromTrack Starting track
		 *  @param toTrack Ending track
		 *  @param loop Whether to play the range of track again after it ends
		 */
		static void Play(const int fromTrack, const int toTrack, const bool loop);

		/** @brief Play single CD track
		 *  @param track Track number (in jo starts at 3)
		 *  @param loop Whether to loop track after it ends
		 */
		static void PlaySingle(const int track, const bool loop)
		{
			PoneSound::CD::Play(track, track, loop);
		}

		/** @brief Stop CD music playback
		 */
		static void Stop();
	};
}
