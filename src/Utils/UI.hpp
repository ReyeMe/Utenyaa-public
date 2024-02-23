#pragma once

#include "std\vector.h"
#include "std\string.h"

#include "Debug.hpp"
#include "Message.hpp"
#include "Settings.hpp"

namespace UI
{
    namespace Messages
    {
        struct NavigatePrevious : MessageType<NavigatePrevious> {};
        struct NavigateNext : MessageType<NavigateNext> {};
        struct SelectPrevious : MessageType<SelectPrevious> {};
        struct SelectNext : MessageType<SelectNext> {};
        struct PerformAction : MessageType<PerformAction> {};
        struct Draw : MessageType<Draw> {};
        struct GetText : MessageType<GetText> { std::string text; };

        struct UpdatePlayer : MessageType<UpdatePlayer>
        {
            size_t index;
            int32_t health;
            size_t powerupType;
        };

        struct UpdateTime : MessageType<UpdateTime>
        {
            size_t currentTime;
            UpdateTime(const size_t& time) : currentTime(time) {}
        };
    }

    struct GUIElement : public IMessageHandler
    {
        virtual void HandleMessages(const Message& message) override {}
    };

    struct Button : public GUIElement
    {
        size_t x = 0;
        size_t y = 0;
        Button(const size_t& posX, const size_t& posY) :x(posX), y(posY) {}

        virtual void HandleMessages(const Message& message) override {}
    };

    struct ActionButton : public Button
    {
        virtual void PerformAction() = 0;
        virtual std::string Text() const = 0;

        void HandleMessages(const Message& message) override
        {
            if (message.IsType<Messages::PerformAction>()) { return PerformAction(); }
            if (auto* getText = message.TryCast<Messages::GetText>()) { getText->text = std::move(Text()); }
        }

        ActionButton(const size_t& posX, const size_t& posY) : Button(posX, posY) {}
    };

    struct Selector : public Button
    {
        virtual void SelectNext() = 0;
        virtual void SelectPrevious() = 0;
        virtual std::string Text() const = 0;

        void HandleMessages(const Message& message) override
        {
            if (message.IsType<Messages::SelectNext>()) { return SelectNext(); }
            if (message.IsType<Messages::SelectPrevious>()) { return SelectPrevious(); }
            if (auto* getText = message.TryCast<Messages::GetText>()) { getText->text = std::move(Text()); }
        }

        Selector(const size_t& posX, const size_t& posY) : Button(posX, posY) {}
    };

    template <typename T>
    concept DerivedFromButton = std::is_base_of<Button, T>::value;

    template <DerivedFromButton... Buttons>
    class ButtonGroup : public GUIElement
    {
    private:
        static constexpr size_t buttonCount = sizeof... (Buttons);
        size_t current = 0;
        Button* buttons[buttonCount];

    public:
        ButtonGroup() : buttons{ new Buttons()... } {}

        ~ButtonGroup() { for (Button* toggle : buttons) { delete toggle; } }

        void NavigatePrevious() { current = (current == 0) ? buttonCount - 1 : current - 1; }

        void NavigateNext() { current = (current + 1) % buttonCount; }

        void Draw()
        {
            Messages::GetText getTextMessage;
            for (size_t i = 0; i < buttonCount; i++)
            {
                buttons[i]->HandleMessages(getTextMessage);
                jo_printf(buttons[i]->x - 2, buttons[i]->y, "%s%s", i == current ? "->" : "  ", getTextMessage.text.c_str());
            }
        }

        void HandleMessages(const Message& message) override
        {
            if (message.IsType<Messages::NavigateNext>()) { return NavigateNext(); }
            if (message.IsType<Messages::NavigatePrevious>()) { return NavigatePrevious(); }
            if (message.IsType<Messages::Draw>()) { return Draw(); }
            buttons[current]->HandleMessages(message);
        }
    };

    struct HUD : public GUIElement
    {
        inline static int headLocations[4][2] = { { 10, 3 }, { 270, 3 }, { 3, 48 }, { 278, 48 } };

        int32_t playerHealth[4] = { 6, 6, 6, 6 };
        size_t powerupType[4] = { 0, 0, 0, 0 };

        void HandleMessages(const Message& message)
        {
            if (auto* playerUpdate = message.TryCast<Messages::UpdatePlayer>())
            {
                playerHealth[playerUpdate->index] = playerUpdate->health;
                powerupType[playerUpdate->index] = playerUpdate->powerupType;
            }

            if (auto* timeUpdate = message.TryCast<Messages::UpdateTime>())
            {
                jo_printf(18, 0, "%01d:%02d", timeUpdate->currentTime / 60, timeUpdate->currentTime % 60);
            }
            Draw();
        }

        void Draw()
        {

			for (size_t player = 0; player < Settings::PlayerCount; player++)
			{
				int offset = 3;

				if (playerHealth[player] <= 0)
				{
					offset = 0;
				}
				else if (playerHealth[player] < 3)
				{
					offset = 1;
				}
				else if (playerHealth[player] < 5)
				{
					offset = 2;
				}

				if (player == 0 || player == 2)
				{
					for(int heart = 0; heart < JO_MIN(playerHealth[player], 3); heart++)
					{
						jo_sprite_draw3D2(2, headLocations[player][X] + 38 + (heart * 8), headLocations[player][Y], 50);
					}

					for(int heart = 3; heart < playerHealth[player]; heart++)
					{
						jo_sprite_draw3D2(2, headLocations[player][X] + 42 + ((heart - 3) * 8), headLocations[player][Y] + 9, 50);
					}
				}
				else if (player == 1 || player == 3)
				{
					for(int heart = 0; heart < JO_MIN(playerHealth[player], 3); heart++)
					{
						jo_sprite_draw3D2(2, headLocations[player][X] - 4 - (heart * 8), headLocations[player][Y], 50);
					}

					for(int heart = 3; heart < playerHealth[player]; heart++)
					{
						jo_sprite_draw3D2(2, headLocations[player][X] - 8 - ((heart - 3) * 8), headLocations[player][Y] + 9, 50);
					}
				}

				if (powerupType[player] > 0 && (player == 0 || player == 2))
				{
					jo_sprite_draw3D2(powerupType[player] - 1, headLocations[player][X] + 40, headLocations[player][Y] + 16, 50);
				}
				else if (powerupType[player] > 0 && (player == 1 || player == 3))
				{
					jo_sprite_draw3D2(powerupType[player] - 1, headLocations[player][X] - 8, headLocations[player][Y] + 16, 50);
				}

				jo_sprite_draw3D2(3 + (player * 4) + offset, headLocations[player][X], headLocations[player][Y], 50);
			}
        }

    };

    static inline HUD HudHandler;
};
