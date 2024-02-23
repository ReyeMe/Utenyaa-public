
#include <jo\Jo.hpp>
#include "utils\ponesound\ponesound.hpp"

#include "Utils\Math\Vec3.hpp"
#include "Utils\TrackableObject.hpp"

#include "Utils\Message.hpp"
#include "Entities\World.hpp"
#include "Utils\Menu.hpp"
#include "Utils\Helpers.hpp"

#include "Utils\Debug.hpp"

jo_camera camera;
int logo;

static jo_palette titleScreen;

/* Palette creation handler
 * @return Pointer to created palette
 */
jo_palette* TgaPaletteHandling(void)
{
	jo_create_palette(&titleScreen);
	return (&titleScreen);
}

int main()
{
	jo_core_init(JO_COLOR_Black);
	slDynamicFrame(1);
	Objects::Terrain::InitColliders();
	IMessageHandler::Init();

	// Random seed
	jo_random_seed = jo_time_get_frc();

	// This magic number fixes the 8bits images on VDP2 not working
	*(volatile unsigned char*)0x060FFCD8 = 0x1F;

	// This will set the screen order
	jo_core_set_screens_order(JO_NBG1_SCREEN, JO_SPRITE_SCREEN);

	PoneSound::Driver::Initialize(PoneSound::ADXMode::ADX2304);

	jo_3d_camera_init(&camera);
	jo_3d_camera_set_z_angle(&camera, 0);
	jo_3d_camera_set_viewpoint(&camera, 0, 20, 120);
	jo_3d_camera_set_target(&camera, 0, 30, 0);

	// Load title screen, we can just leave this on NBG2 and turn that layer off when we do not want to see it
	jo_img_8bits bg;
	bg.data = NULL;
	jo_set_tga_palette_handling(TgaPaletteHandling);
	jo_tga_8bits_loader(&bg, JO_ROOT_DIR, "LOGO.TGA", 1); // Thx to jo's wisdom the colors in the image palette start with 1
	jo_set_tga_palette_handling(nullptr);
	jo_set_background_8bits_sprite(&bg, titleScreen.id, false);
	jo_free_img(&bg);

	// Load sounds
	PoneSound::Sound::LoadPcm((char*)"BOMB.PCM", PoneSound::PCMBitDepth::PCM8, 15360);
	PoneSound::Sound::LoadPcm((char*)"CANNON.PCM", PoneSound::PCMBitDepth::PCM8, 15360);

	PoneSound::Sound::LoadPcm((char*)"CRFALL.PCM", PoneSound::PCMBitDepth::PCM8, 15360);
	PoneSound::Sound::LoadPcm((char*)"CROPEN.PCM", PoneSound::PCMBitDepth::PCM8, 15360);
	PoneSound::Sound::LoadPcm((char*)"CRSPAWN.PCM", PoneSound::PCMBitDepth::PCM8, 15360);

	PoneSound::Sound::LoadPcm((char*)"MHOV.PCM", PoneSound::PCMBitDepth::PCM8, 15360);
	PoneSound::Sound::LoadPcm((char*)"MSEL.PCM", PoneSound::PCMBitDepth::PCM8, 15360);

	PakTextureLoader::LoadTextures("HUD.PAK");

	Objects::Terrain::FirstGroundTextureIndex = PakTextureLoader::LoadTextures("TERRAIN.PAK");
	Entities::Player::SetTextureId(PakTextureLoader::LoadTextures("CHARS.PAK"));
	Entities::Explosion::SetTextureId(PakTextureLoader::LoadTextures("EXP.PAK"));

	// Load additional textures
	int first = PakTextureLoader::LoadTextures("WEAP.PAK");
	Entities::Bullet::SetTextureId(first);
	Entities::Mine::SetTextureId(first + 1);

	// Load models
	ModelManager::LoadModel((char*)"CRATE.NYA");
	ModelManager::LoadModel((char*)"PLAYER.NYA");
	ModelManager::LoadModel((char*)"TREE.NYA");
	ModelManager::LoadModel((char*)"WALL.NYA");
	ModelManager::LoadModel((char*)"WALL2.NYA");
	ModelManager::LoadModel((char*)"WALL3.NYA");
	ModelManager::LoadModel((char*)"BOMB.NYA");

	PoneSound::CD::Play(2, 2, true);

	Entities::World* worldPtr = nullptr;
	Fxp startTime = 0.0;
	
	while (1)
	{
		jo_fixed_point_time();

		static UI::Menu menu;
		menu.Update();

		if (Settings::Quit && worldPtr)
		{
			PoneSound::CD::Play(2, 2, true);
			for (auto* object : IRenderable::objects) delete object;
			worldPtr = nullptr;
			Settings::Quit = false;
			Settings::GameEnded = false;
		}

		if (Settings::IsActive)
		{

			if (worldPtr == nullptr)
			{
				Settings::GameEnded = false;
				startTime = Fxp::FromInt(Settings::TotalSeconds);
				worldPtr = new Entities::World(Settings::StageFiles[Settings::SelectedStage]);
				PoneSound::CD::Play(3, 3, true);
			}
			slUnitMatrix(0);

			// Update entities
			for (auto* object : IUpdatable::objects) object->Update();

			jo_3d_camera_look_at(&camera);
			jo_3d_push_matrix();
			{
				jo_3d_rotate_matrix_rad_x(0.5f);
				jo_3d_translate_matrix_fixed(-10 << 19, -10 << 19, 0);

				// Draw entities onto the world
				for (auto* object : IRenderable::objects) object->Draw();
			}
			jo_3d_pop_matrix();

			Helpers::HideLogo();

			int alive = 0;

			for (auto* object : TrackableObject<Entities::Player>::objects)
			{
				if (object->GetHealth() > 0)
				{
					alive++;
				}
			}	

			if (startTime <= 0.0 || alive <= 1)
			{
				// Show match results
				Settings::IsActive = false;
				Settings::GameEnded = true;

				int winner = 0;
				int maxHealth = 0;

				for (auto* object : TrackableObject<Entities::Player>::objects)
				{
					int health = object->GetHealth();

					if (health > maxHealth)
					{
						maxHealth = health;
						winner = object->GetController();
					}
				}

				Settings::PlayerWon = winner + 1;
				PoneSound::CD::Play(4, 4, false);
			}
			else
			{
				UI::HudHandler.HandleMessages(UI::Messages::UpdateTime((startTime >> 16).Value()));
				startTime -= Fxp::BuildRaw(delta_time);
			}
		}
		else
		{
			Helpers::ShowLogo();
		}


		slSynch();
	}
	return 0;
}
