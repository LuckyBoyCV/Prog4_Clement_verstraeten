#pragma once
#include <memory>
#include <vector>
#include <SDL3/SDL.h>
#include "Singleton.h"
#include "Command.h"
#include "Controller.h"

namespace dae
{
	enum class KeyState
	{
		Down,
		Up,
		Pressed,
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput(float deltaTime);

		void BindKeyboardCommand(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command);
		void UnbindKeyboardCommand(SDL_Scancode key, KeyState state);

		void BindControllerCommand(int controllerIndex, Controller::button button, KeyState state, std::unique_ptr<Command> command);
		void UnbindControllerCommand(int controllerIndex, Controller::button button, KeyState state);

	private:
		bool IsKeyDown(SDL_Scancode key) const;
		bool IsKeyUp(SDL_Scancode key) const;
		bool IsKeyPressed(SDL_Scancode key) const;

		const bool* m_pKeyboardState{ nullptr };
		std::vector<bool> m_previousKeyboardState{};

		struct KeyboardBinding
		{
			SDL_Scancode key;
			KeyState state;
			std::unique_ptr<Command> command;
		};

		struct ControllerBinding
		{
			int controllerIndex;
			Controller::button button;
			KeyState state;
			std::unique_ptr<Command> command;
		};

		std::vector<KeyboardBinding> m_keyboardBindings{};
		std::vector<ControllerBinding> m_controllerBindings{};
		std::vector<std::unique_ptr<Controller>> m_controllers{};

		Controller* GetController(int index);
	};
}