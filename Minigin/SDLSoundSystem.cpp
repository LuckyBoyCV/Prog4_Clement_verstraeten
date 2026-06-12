#include "SDLSoundSystem.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <thread>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <condition_variable>
#include <atomic>
#include <iostream>

namespace dae
{
    struct PlaySoundRequest
    {
        sound_id id;
        float volume;
    };

    class SDLSoundSystem::Impl
    {
    public:
        Impl()
        {
            if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
            {
                std::cout << "Failed to init SDL audio: " << SDL_GetError() << "\n";
                return;
            }

            if (!MIX_Init())
            {
                std::cout << "Failed to initialize SDL_mixer: " << SDL_GetError() << "\n";
                return;
            }

            m_pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
            if (!m_pMixer)
            {
                std::cout << "Failed to create mixer device: " << SDL_GetError() << "\n";
                return;
            }

            m_workerThread = std::jthread(&Impl::HandleQueue, this);
            std::cout << "Sound system initialized\n";
        }

        ~Impl()
        {
            {
                std::lock_guard lock(m_queueMutex);
                m_active = false;
            }
            m_wakeUp.notify_one();
            if (m_workerThread.joinable())
                m_workerThread.join();

            for (MIX_Track* pTrack : m_activeTracks)
            {
                if (pTrack)
                    MIX_DestroyTrack(pTrack);
            }
            m_activeTracks.clear();

            for (auto& [id, pAudio] : m_loadedAudio)
            {
                if (pAudio)
                {
                    MIX_DestroyAudio(pAudio);
                    pAudio = nullptr;
                }
            }
            m_loadedAudio.clear();

            if (m_pMixer)
            {
                SDL_CloseAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK);
                m_pMixer = nullptr;
            }

            std::cout << "Sound system shut down\n";
        }

        Impl(const Impl&) = delete;
        Impl(Impl&&) = delete;
        Impl& operator=(const Impl&) = delete;
        Impl& operator=(Impl&&) = delete;

        void RequestPlay(sound_id id, float volume)
        {
            // muted: drop the request rather than queue silence
            if (m_muted)
                return;

            {
                std::lock_guard lock(m_queueMutex);
                m_pendingRequests.push({ id, volume });
            }
            m_wakeUp.notify_one();
        }

        void SetMuted(bool muted) { m_muted = muted; }
        bool IsMuted() const { return m_muted; }

        sound_id RegisterSound(const std::string& filePath)
        {
            sound_id newId{};
            {
                std::lock_guard lock(m_queueMutex);
                newId = m_nextId;
                ++m_nextId;
                m_soundPaths[newId] = filePath;
            }
            std::cout << "Registered sound [" << newId << "]: " << filePath << "\n";
            return newId;
        }

    private:
        void HandleQueue()
        {
            while (true)
            {
                std::unique_lock lock(m_queueMutex);
                m_wakeUp.wait(lock, [this]()
                    {
                        return !m_pendingRequests.empty() || !m_active;
                    });

                while (!m_pendingRequests.empty())
                {
                    PlaySoundRequest req{ m_pendingRequests.front() };
                    m_pendingRequests.pop();

                    // grab the path while we still hold the lock so the worker never touches m_soundPaths unlocked
                    auto pathIt = m_soundPaths.find(req.id);
                    std::string path{ pathIt != m_soundPaths.end() ? pathIt->second : std::string{} };

                    lock.unlock();

                    if (m_loadedAudio.find(req.id) == m_loadedAudio.end())
                    {
                        std::cout << "Loading sound [" << req.id << "]: " << path << "\n";
                        m_loadedAudio[req.id] = MIX_LoadAudio(m_pMixer, path.c_str(), false);
                    }

                    MIX_Audio* pAudio = m_loadedAudio[req.id];
                    if (pAudio)
                    {
                        MIX_Track* pTrack = MIX_CreateTrack(m_pMixer);
                        MIX_SetTrackAudio(pTrack, pAudio);
                        MIX_SetTrackGain(pTrack, req.volume);
                        MIX_PlayTrack(pTrack, 0);
                        m_activeTracks.push_back(pTrack);
                        std::cout << "Playing sound [" << req.id << "] at volume " << req.volume << "\n";
                    }
                    else
                    {
                        std::cout << "Failed to play sound [" << req.id << "]: " << SDL_GetError() << "\n";
                    }

                    lock.lock();
                }

                if (!m_active)
                    break;
            }
        }

        std::jthread m_workerThread{};
        MIX_Mixer* m_pMixer{ nullptr };
        sound_id m_nextId{};

        std::map<sound_id, std::string> m_soundPaths;
        std::map<sound_id, MIX_Audio*> m_loadedAudio;

        bool m_active{ true };
        std::atomic<bool> m_muted{ false };
        std::mutex m_queueMutex{};
        std::queue<PlaySoundRequest> m_pendingRequests{};
        std::condition_variable m_wakeUp{};

        std::vector<MIX_Track*> m_activeTracks{};
    };
}

dae::SDLSoundSystem::SDLSoundSystem()
    : m_pImpl(std::make_unique<Impl>())
{
}

dae::SDLSoundSystem::~SDLSoundSystem() = default;

void dae::SDLSoundSystem::Play(sound_id id, float volume)
{
    m_pImpl->RequestPlay(id, volume);
}

dae::sound_id dae::SDLSoundSystem::RegisterSound(const std::string& filePath)
{
    return m_pImpl->RegisterSound(filePath);
}

void dae::SDLSoundSystem::SetMuted(bool muted)
{
    m_pImpl->SetMuted(muted);
}

bool dae::SDLSoundSystem::IsMuted() const
{
    return m_pImpl->IsMuted();
}