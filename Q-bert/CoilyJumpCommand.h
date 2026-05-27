#pragma once
#include "Command.h"

namespace qbert
{
	class coilyJumpCommand final : public dae::GameActorCommand
	{
	public:
		coilyJumpCommand(dae::GameObject* pOwner, int destRow, int destCol)
			: GameActorCommand(pOwner), m_destRow(destRow), m_destCol(destCol) {}

		void Execute() override;

	private:
		int m_destRow;
		int m_destCol;
	};
}
