#pragma once
#include "Component.h"
#include "Observer.h"
#include "Subject.h"
#include "RenderComponent.h"

namespace qbert
{
	class PyramidComponent;
	class QbertComponent;

	class SlickSamComponent final : public dae::Component, public dae::Observer
	{

	public:
		explicit SlickSamComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, QbertComponent* qbert);
		~SlickSamComponent() override = default;
		SlickSamComponent(const SlickSamComponent&) = delete;
		SlickSamComponent(SlickSamComponent&&) = delete;
		SlickSamComponent& operator=(const SlickSamComponent&) = delete;
		SlickSamComponent& operator=(SlickSamComponent&&) = delete;

		void Update(float deltaTime) override;
		bool isJumping() const { return m_isJumping; }
		bool Move(int destRow, int destCol);

		void onTile();
		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;

		dae::Subject m_subject;

	private:
		void Respawn();

		PyramidComponent* m_pPyramid;
		QbertComponent* m_pQbert;
		int   m_row{ 0 };
		int   m_col{ 0 };
		int   m_futureRow{ 0 };
		int   m_futureCol{ 0 };

		bool  m_isJumping{ false };
		bool  m_waitingToRespawn{ false };

		float m_jumpTime{ 0.f };
		float m_respawnTimer{ 0.f };
		float m_respawnDelay{ 3.f };

		float m_jumpDuration{ 0.2f };
		float m_jumpInterval{ 1.0f };
		float m_respawnDuration{ 3.f };
		bool  m_qbertWasJumping{ false };

		int   m_respawnRow{ 0 };
		int   m_respawnCol{ 0 };

		float m_spriteOffsetY{ 25.f };
		float m_spriteOffsetX{ 30.f };

	};

}
