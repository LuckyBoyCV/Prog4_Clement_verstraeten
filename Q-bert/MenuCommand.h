#pragma once
#include "Command.h"
#include "mainMenuComponent.h"

namespace qbert
{
	class MenuNavigateCommand final : public dae::Command
	{
	public:
		MenuNavigateCommand(MainMenuComponent* menu, int direction)
			: m_menu(menu), m_direction(direction) {}

		void Execute() override
		{
			if (m_direction < 0) m_menu->SelectUp();
			else                 m_menu->SelectDown();
		}

	private:
		MainMenuComponent* m_menu;
		int m_direction;
	};

	class MenuConfirmCommand final : public dae::Command
	{
	public:
		explicit MenuConfirmCommand(MainMenuComponent* menu) : m_menu(menu) {}
		void Execute() override { m_menu->Confirm(); }

	private:
		MainMenuComponent* m_menu;
	};
}
