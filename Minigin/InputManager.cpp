#include "InputManager.h"
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include <algorithm>

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
			return false;

		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	m_pKeyboardState = SDL_GetKeyboardState(nullptr);

	if (m_previousKeyboardState.empty())
		m_previousKeyboardState.assign(SDL_SCANCODE_COUNT, false);

	for (auto& controller : m_controllers)
		controller->Update();

	for (const auto& binding : m_keyboardBindings)
	{
		bool shouldExecute = false;

		if (binding.state == KeyState::Down)
			shouldExecute = IsKeyDown(binding.key);
		else if (binding.state == KeyState::Up)
			shouldExecute = IsKeyUp(binding.key);
		else if (binding.state == KeyState::Pressed)
			shouldExecute = IsKeyPressed(binding.key);

		if (shouldExecute)
			binding.command->Execute();
	}

	for (const auto& binding : m_controllerBindings)
	{
		Controller* pController = GetController(binding.controllerIndex);

		if (!pController->IsConnected())
			continue;

		bool shouldExecute = false;

		if (binding.state == KeyState::Down)
			shouldExecute = pController->IsDownThisFrame(binding.button);
		else if (binding.state == KeyState::Up)
			shouldExecute = pController->IsUpThisFrame(binding.button);
		else if (binding.state == KeyState::Pressed)
			shouldExecute = pController->IsPressed(binding.button);

		if (shouldExecute)
			binding.command->Execute();
	}

	for (int i = 0; i < SDL_SCANCODE_COUNT; ++i)
		m_previousKeyboardState[i] = m_pKeyboardState[i];

	return true;
}

bool dae::InputManager::IsKeyDown(SDL_Scancode key) const
{
	bool currentlyPressed = m_pKeyboardState[key];
	bool wasPressed = m_previousKeyboardState[key];
	return currentlyPressed && !wasPressed;
}

bool dae::InputManager::IsKeyUp(SDL_Scancode key) const
{
	bool currentlyPressed = m_pKeyboardState[key];
	bool wasPressed = m_previousKeyboardState[key];
	return !currentlyPressed && wasPressed;
}

bool dae::InputManager::IsKeyPressed(SDL_Scancode key) const
{
	return m_pKeyboardState[key];
}

void dae::InputManager::BindKeyboardCommand(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command)
{
	m_keyboardBindings.push_back({ key, state, std::move(command) });
}

void dae::InputManager::UnbindKeyboardCommand(SDL_Scancode key, KeyState state)
{
	m_keyboardBindings.erase(
		std::remove_if(m_keyboardBindings.begin(), m_keyboardBindings.end(),
			[key, state](const KeyboardBinding& binding)
			{
				return binding.key == key && binding.state == state;
			}),
		m_keyboardBindings.end());
}

void dae::InputManager::BindControllerCommand(int controllerIndex, Controller::button button, KeyState state, std::unique_ptr<Command> command)
{
	GetController(controllerIndex);
	m_controllerBindings.push_back({ controllerIndex, button, state, std::move(command) });
}

void dae::InputManager::UnbindControllerCommand(int controllerIndex, Controller::button button, KeyState state)
{
	m_controllerBindings.erase(
		std::remove_if(m_controllerBindings.begin(), m_controllerBindings.end(),
			[controllerIndex, button, state](const ControllerBinding& binding)
			{
				return binding.controllerIndex == controllerIndex
					&& binding.button == button
					&& binding.state == state;
			}),
		m_controllerBindings.end());
}

dae::Controller* dae::InputManager::GetController(int index)
{
	while (static_cast<int>(m_controllers.size()) <= index)
		m_controllers.push_back(std::make_unique<Controller>(static_cast<unsigned int>(m_controllers.size())));

	return m_controllers[index].get();
}