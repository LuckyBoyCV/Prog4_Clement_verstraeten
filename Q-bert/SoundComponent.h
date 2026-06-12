#pragma once
#include "Component.h"
#include "Observer.h"
#include "SoundSystem.h"

namespace qbert
{
	// reacts to qbert events and plays jump.wav on a move and death.wav on a death
	class SoundComponent final : public dae::Component, public dae::Observer
	{
	public:
		SoundComponent(dae::GameObject* pOwner, dae::sound_id jumpSound, dae::sound_id deathSound, float volume = 1.f);
		~SoundComponent() override = default;
		SoundComponent(const SoundComponent&) = delete;
		SoundComponent(SoundComponent&&) = delete;
		SoundComponent& operator=(const SoundComponent&) = delete;
		SoundComponent& operator=(SoundComponent&&) = delete;

		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;

	private:
		dae::sound_id m_jumpSound;
		dae::sound_id m_deathSound;
		float m_volume;
	};
}
