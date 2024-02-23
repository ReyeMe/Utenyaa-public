#pragma once

#include "Settings.hpp"
#include "UI.hpp"

namespace UI
{
    class Menu
    {
        // Main Menu

        struct StageSelector : Settings, Selector
        {
            std::string Text() const { return std::string("%-13s< %s >", "Stage Select", StageNames[SelectedStage]); }
            void SelectNext() { SelectedStage = SelectedStage < (StageCount - 1) ? SelectedStage + 1 : 0; }
            void SelectPrevious() { SelectedStage = SelectedStage > 0 ? SelectedStage - 1 : StageCount - 1; }
            StageSelector() : Selector(9, 15) {}
        };

        struct PlayerCountSelector : Settings, Selector
        {
            std::string Text() const { return std::string("%-13s< %d >", "Player Count", PlayerCount); }
            void SelectNext() { PlayerCount = PlayerCount < MaxPlayerCount ? PlayerCount + 1 : 2; }
            void SelectPrevious() { PlayerCount = PlayerCount > 2 ? PlayerCount - 1 : MaxPlayerCount; }
            PlayerCountSelector() : Selector(9, 16) {}
        };

        struct TimeLimitSelector : Settings, Selector
        {
            std::string Text() const { return std::string("%-13s< %01dm:%02ds >", "Time Limit", TotalSeconds / 60, TotalSeconds % 60); }
            void SelectNext() { ++TotalSeconds; }
            void SelectPrevious() { if (TotalSeconds > 0) --TotalSeconds; }
            TimeLimitSelector() : Selector(9, 17) {}
        };

        struct GoToIntro : Settings, ActionButton
        {
            std::string Text() const { return "Go Back"; }
            void PerformAction() { currentScreen = Screen::Intro; }
            GoToIntro() : ActionButton(3, 24) {}
        };

        struct StartGame : Settings, ActionButton
        {
            std::string Text() const { return "Start Game"; }

            void PerformAction()
            {
                Settings::IsActive = true;
            }

            StartGame() : ActionButton(15, 24) {}
        };

        // Intro Menu

        struct GoToMainMenu : Settings, ActionButton
        {
            std::string Text() const { return "Play"; }
            void PerformAction() { currentScreen = Screen::MainMenu; }
            GoToMainMenu() : ActionButton(9, 15) {}
        };

        struct GoToCredits : Settings, ActionButton
        {
            std::string Text() const { return "Credits"; }
            void PerformAction() { currentScreen = Screen::Credits; }
            GoToCredits() : ActionButton(9, 16) {}
        };

        // Pause menu

        struct Unpause : Settings, ActionButton
        {
            std::string Text() const { return "Return to Game"; }
            void PerformAction() { Settings::IsActive = true; }
            Unpause() : ActionButton(9, 15) {}
        };

        struct Quit : Settings, ActionButton
        {
            std::string Text() const { return "Quit Game"; }
            void PerformAction()
            {
                currentScreen = Screen::Intro;
                Settings::IsActive = false;
                Settings::Quit = true;
				Settings::GameEnded = false;
            }
            Quit() : ActionButton(9, 16) {}
        };

        // End screen menu
		
        struct EndScreenText : Settings, ActionButton
        {
            std::string Text() const { return std::string("Player %d won!", Settings::PlayerWon); }
            void PerformAction() {}
            EndScreenText() : ActionButton(9, 15) {}
        };

        // Credits menu
        struct ReyMe : Settings, ActionButton
        {
            std::string Text() const { return "ReyMe"; }
            void PerformAction() { currentScreen = Screen::Intro; }
            ReyMe() : ActionButton(9, 15) {}
        };

        struct DannyDuarte : Settings, ActionButton
        {
            std::string Text() const { return "DannyDuarte"; }
            void PerformAction() { currentScreen = Screen::Intro; }
            DannyDuarte() : ActionButton(9, 16) {}
        };

        struct am25 : Settings, ActionButton
        {
            std::string Text() const { return "am.25"; }
            void PerformAction() { currentScreen = Screen::Intro; }
            am25() : ActionButton(9, 17) {}
        };

        struct Random : Settings, ActionButton
        {
            std::string Text() const { return "Random"; }
            void PerformAction() { currentScreen = Screen::Intro; }
            Random() : ActionButton(9, 18) {}
        };

        struct AnriFox : Settings, ActionButton
        {
            std::string Text() const { return "AnriFox"; }
            void PerformAction() { currentScreen = Screen::Intro; }
            AnriFox() : ActionButton(9, 19) {}
        };

        enum Screen
        {
            Intro,
            Credits,
            MainMenu,
            Pause,
			GameEnd,
            TotalScreens
        };

        static inline Screen currentScreen = Screen::Intro;

        GUIElement* screens[TotalScreens] =
        {
            new ButtonGroup<GoToMainMenu, GoToCredits>,
            new ButtonGroup<ReyMe, DannyDuarte, am25, Random, AnriFox>,
            new ButtonGroup<StageSelector, PlayerCountSelector, TimeLimitSelector, GoToIntro, StartGame>,
            new ButtonGroup<Unpause,Quit>,
            new ButtonGroup<EndScreenText,Quit>
        };

    public:
        void ProcessInput()
        {
            if (!Settings::IsActive)
            {
                if (jo_is_pad1_key_down(JO_KEY_A))
                {
                    screens[currentScreen]->HandleMessages(Messages::PerformAction());
                }
                if (jo_is_pad1_key_down(JO_KEY_DOWN))
                {
                    screens[currentScreen]->HandleMessages(Messages::NavigateNext());
                }
                if (jo_is_pad1_key_down(JO_KEY_UP))
                {
                    screens[currentScreen]->HandleMessages(Messages::NavigatePrevious());
                }
                if (jo_is_pad1_key_down(JO_KEY_LEFT))
                {
                    screens[currentScreen]->HandleMessages(Messages::SelectPrevious());
                }
                if (jo_is_pad1_key_down(JO_KEY_RIGHT))
                {
                    screens[currentScreen]->HandleMessages(Messages::SelectNext());
                }
            }
        }

        void Update()
        {
			if (!Settings::IsActive && Settings::GameEnded)
			{
                currentScreen = Screen::GameEnd;
                ProcessInput();
                jo_clear_screen();
                screens[currentScreen]->HandleMessages(Messages::Draw());
			}
            else if (Settings::IsActive && jo_is_pad1_key_down(JO_KEY_START))
            {
                Settings::IsActive = false;
                currentScreen = Screen::Pause;
            }
            else
            {
                ProcessInput();
                jo_clear_screen();
                screens[currentScreen]->HandleMessages(Messages::Draw());
                if (Settings::IsActive) jo_clear_screen();
            }
        }
    };
}
