#pragma once
#include "Command.h"
#include "ServiceLocator.h"

namespace dae
{
    class SoundCommand final : public Command
    {
    public:
        SoundCommand(sound_id id, float volume)
            : m_id(id), m_volume(volume) {
        }

        void Execute() override
        {
            ServiceLocator::GetSoundSystem().Play(m_id, m_volume);
        }

    private:
        sound_id m_id;
        float m_volume;
    };
}