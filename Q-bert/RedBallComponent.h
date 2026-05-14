#pragma once
#include "Component.h"
#include "RenderComponent.h"

namespace qbert
{

	class PyramidComponent;
	class QbertComponent;


	class redBallComponent final : public dae::Component
	{
	public:
		explicit redBallComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, QbertComponent* qbert);
		~redBallComponent() override = default;
		redBallComponent(const redBallComponent&) = delete;
		redBallComponent(redBallComponent&&) = delete;
		redBallComponent& operator=(const redBallComponent&) = delete;
		redBallComponent& operator=(redBallComponent&&) = delete;

		void Update(float deltaTime) override;
		bool Move(int destRow, int destCol);
		bool isJumping() const { return m_isJumping; }
		void setSprite(int x, int y, int w, int h);



	private:
		void Respawn();

		PyramidComponent* m_pPyramid;
		QbertComponent* m_pQbert;
		int m_row{ 0 };
		int m_col{ 0 };
		bool m_isJumping{ false };

		int m_futureRow{ 0 };
		int m_futureCol{ 0 };

		float m_jumpTime{ 0.f };
		float m_jumpDuration{ 0.2f };
		float m_jumpInterval{ 0.8f };

		float respawnTime{ 0.f };
		float respawnDuration{ 3.f };


	};
}