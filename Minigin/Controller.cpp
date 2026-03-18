#include "Controller.h"
#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib,"xinput.lib")

namespace dae
{
    class Controller::ControllerImpl
    {
    public:
        explicit ControllerImpl(unsigned int index) : m_controllerIndex(index)
        {
            ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_previousState, sizeof(XINPUT_STATE));
        }

        void update()
        {
            CopyMemory(&m_previousState, &m_currentState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));
            m_isConnected = XInputGetState(m_controllerIndex, &m_currentState) == ERROR_SUCCESS;
            if (!m_isConnected) return;
            const WORD buttonsChanged = m_currentState.Gamepad.wButtons ^ m_previousState.Gamepad.wButtons;
            m_ButtonsPressedThisFrame = buttonsChanged & m_currentState.Gamepad.wButtons;
            m_ButtonsReleasedThisFrame = buttonsChanged & ~m_currentState.Gamepad.wButtons;
        }

        bool IsDownThisFrame(unsigned int button) const { return (m_ButtonsPressedThisFrame & button) != 0; }
        bool IsUpThisFrame(unsigned int button) const { return (m_ButtonsReleasedThisFrame & button) != 0; }
        bool IsPressed(unsigned int button) const { return (m_currentState.Gamepad.wButtons & button) != 0; }
        bool IsConnected()        const { return m_isConnected; }
        unsigned int GetControllerIndex() const { return m_controllerIndex; }

    private:
        XINPUT_STATE m_currentState{};
        XINPUT_STATE m_previousState{};
        unsigned int m_controllerIndex{};
        bool m_isConnected{ false };
        WORD m_ButtonsPressedThisFrame{};
        WORD m_ButtonsReleasedThisFrame{};
    };
}

#else

namespace dae
{
    class Controller::ControllerImpl
    {
    public:
        explicit ControllerImpl(unsigned int index) : m_controllerIndex(index) {}
        void update() {}
        bool IsDownThisFrame(unsigned int) const { return false; }
        bool IsUpThisFrame(unsigned int) const { return false; }
        bool IsPressed(unsigned int) const { return false; }
        bool IsConnected()        const { return false; }
        unsigned int GetControllerIndex() const { return m_controllerIndex; }
    private:
        unsigned int m_controllerIndex{};
    };
}
#endif

namespace dae
{
    Controller::Controller(unsigned int controllerIndex)
        : m_pImpl(std::make_unique<ControllerImpl>(controllerIndex)) {
    }

    Controller::~Controller() = default;
    void Controller::Update() { m_pImpl->update(); }
    bool Controller::IsDownThisFrame(button b) const { return m_pImpl->IsDownThisFrame(static_cast<unsigned int>(b)); }
    bool Controller::IsUpThisFrame(button b) const { return m_pImpl->IsUpThisFrame(static_cast<unsigned int>(b)); }
    bool Controller::IsPressed(button b) const { return m_pImpl->IsPressed(static_cast<unsigned int>(b)); }
    bool Controller::IsConnected()             const { return m_pImpl->IsConnected(); }
    unsigned int Controller::GetControllerIndex() const { return m_pImpl->GetControllerIndex(); }
}