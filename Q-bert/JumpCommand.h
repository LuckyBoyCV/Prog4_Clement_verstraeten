#pragma once
#include "Command.h"

namespace qbert
{
	class QbertComponent;

	class JumpCommand final : public dae::GameActorCommand
	{
	public:
		JumpCommand(dae::GameObject* pOwner, int destRow, int destCol)
			:GameActorCommand(pOwner),
			m_destRow(destRow),
			m_destCol(destCol)
		{ 
		
		}

		void Execute() override;

	private:
		int m_destRow;
		int m_destCol;
	
	};
}