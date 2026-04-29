#pragma once
#include "SoundSystem.h"
#include <string>
#include <memory>

namespace dae
{
    class NullSoundSystem final : public SoundSystem
    {
    public:
        void Play(sound_id, float) override {}
        sound_id RegisterSound(const std::string&) override { return 0; }
    };

    class ServiceLocator final
    {
    public:
        static SoundSystem& GetSoundSystem() { return *m_soundSystem; }

        static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss)
        {
            m_soundSystem = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
        }

    private:
        static std::unique_ptr<SoundSystem> m_soundSystem;
    };
}