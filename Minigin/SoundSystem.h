#pragma once
#include <string>
namespace dae
{
    using sound_id = unsigned short;

    class SoundSystem
    {
    public:
        virtual ~SoundSystem() = default;
        virtual void Play(sound_id id, float volume) = 0;
        virtual sound_id RegisterSound(const std::string& filePath) = 0;
        virtual void SetMuted(bool muted) = 0;
        virtual bool IsMuted() const = 0;
    };

   
}