#pragma once
#include "Command.h"
#include <glm/glm.hpp>

namespace dae
{
	class MoveCommand final : public GameActorCommand
	{
	public:
		MoveCommand(GameObject* pOwner, glm::vec3 direction, float speed)
			: GameActorCommand(pOwner)
			, m_direction(direction)
			, m_speed(speed)
		{
		}

		void Execute() override;

	private:
		glm::vec3 m_direction;
		float m_speed;
	};
}