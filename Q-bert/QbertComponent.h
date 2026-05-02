#pragma once
#include "Component.h"
#include "Subject.h"

namespace qbert
{



	class PyramidComponent;

	class QbertComponent final : public dae::Component
	{
	public:
		explicit QbertComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, int startRow, int startCol);
		~QbertComponent() override = default;
		QbertComponent(const QbertComponent&) = delete;
		QbertComponent(QbertComponent&&) = delete;
		QbertComponent& operator=(const QbertComponent&) = delete;
		QbertComponent& operator=(QbertComponent&&) = delete;

		void Update(float deltaTime) override;
		bool Move(int destRow, int destCol);
		bool isJumping() const { return m_isJumping; }
		void loseLife();
		int getLives() const { return m_Lives; }

		int getRow() const { return m_row; }
		int getCol() const { return m_col; }

		dae::Subject m_subject;

	private:
		void Respawn();

		PyramidComponent* m_pPyramid;
		int m_Lives;
		int m_row;
		int m_col;
		bool m_isJumping{ false };
		float m_jumpTime{ 0.f };
		float m_jumpDuration{ 0.2f };
		int m_futureRow{ 0 };
		int m_futureCol{ 0 };

		bool m_isFalling{ false };
		float m_respawnTimer{ 0.f };
		float m_respawnDuration{ 2.f };



	};
}