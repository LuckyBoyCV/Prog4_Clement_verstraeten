#include "soundComponent.h"
#include "ServiceLocator.h"
#include "GameEvent.h"

qbert::SoundComponent::SoundComponent(dae::GameObject* pOwner, dae::sound_id jumpSound, dae::sound_id deathSound, float volume)
	: Component(pOwner)
	, m_jumpSound(jumpSound)
	, m_deathSound(deathSound)
	, m_volume(volume)
{
}

void qbert::SoundComponent::OnNotify(dae::GameEvent event, dae::GameObject*)
{
	if (event == dae::GameEvent::PlayerJumped)
		dae::ServiceLocator::GetSoundSystem().Play(m_jumpSound, m_volume);
	else if (event == dae::GameEvent::PlayerDied || event == dae::GameEvent::GameOver)
		dae::ServiceLocator::GetSoundSystem().Play(m_deathSound, m_volume);
}
