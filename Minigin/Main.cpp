#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "RotatorComponent.h"
#include "ThrashCacheComponent.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "Controller.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	// Background
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("background.png");
	scene.Add(std::move(go));

	// Logo
	go = std::make_unique<dae::GameObject>();
	go->SetPosition(358, 180);
	go->AddComponent<dae::RenderComponent>("logo.png");
	scene.Add(std::move(go));

	// Title
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_unique<dae::GameObject>();
	go->SetPosition(292, 20);
	auto* text = go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	text->SetColor({ 255, 255, 255, 255 });
	scene.Add(std::move(go));

	// FPS counter
	auto fpsFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_unique<dae::GameObject>();
	go->SetPosition(5, 5);
	auto* fpsText = go->AddComponent<dae::TextComponent>("0.0 FPS", fpsFont);
	fpsText->SetColor({ 255, 255, 255, 255 });
	go->AddComponent<dae::FPSComponent>();
	scene.Add(std::move(go));

	//Rotating objects
	//auto pivot = std::make_unique<dae::GameObject>();
	//pivot->SetLocalPostion(glm::vec3(512.f, 300.f, 0.f));

	//auto parentChar = std::make_unique<dae::GameObject>();
	//auto* qbert = parentChar->AddComponent<dae::RenderComponent>("qbert.png");
	//qbert->SetScale(0.1f);
	//parentChar->AddComponent<dae::RotatorComponent>(10.f, 4.0f);
	//parentChar->SetParent(pivot.get(), false);


	//auto childChar = std::make_unique<dae::GameObject>();
	//auto* qbert_knight = childChar->AddComponent<dae::RenderComponent>("qbert knight.png");
	//qbert_knight->SetScale(0.1f);
	//childChar->AddComponent<dae::RotatorComponent>(40.f, -3.0f);
	//childChar->SetParent(parentChar.get(), false);


	//cache
	auto cache = std::make_unique<dae::GameObject>();
	cache->AddComponent<dae::ThrashCacheComponent>();


	//Move command
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	go = std::make_unique<dae::GameObject>();
	go->SetPosition(5, 50);
	go->AddComponent<dae::TextComponent>("Use the D-Pad to move Qbert", smallFont)->SetColor({ 255, 200, 0, 255 });
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->SetPosition(5, 75);
	go->AddComponent<dae::TextComponent>("Use WASD to move Qbert Knight", smallFont)->SetColor({ 200, 100, 255, 255 });
	scene.Add(std::move(go));

	auto qbert = std::make_unique<dae::GameObject>();
	qbert->SetPosition(300, 300);
	qbert->AddComponent<dae::RenderComponent>("qbert.png")->SetScale(0.15f);
	dae::GameObject* pQbert = qbert.get();
	scene.Add(std::move(qbert));

	auto qbertKnight = std::make_unique<dae::GameObject>();
	qbertKnight->SetPosition(450, 300);
	qbertKnight->AddComponent<dae::RenderComponent>("qbert knight.png")->SetScale(0.15f);
	dae::GameObject* pQbertKnight = qbertKnight.get();
	scene.Add(std::move(qbertKnight));

	auto& input = dae::InputManager::GetInstance();

	const float speed = 2.f;
	const float doubleSpeed = 4.f;

	input.BindKeyboardCommand(SDL_SCANCODE_W, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pQbertKnight, glm::vec3{ 0, -1, 0 }, speed));
	input.BindKeyboardCommand(SDL_SCANCODE_S, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pQbertKnight, glm::vec3{ 0,  1, 0 }, speed));
	input.BindKeyboardCommand(SDL_SCANCODE_A, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pQbertKnight, glm::vec3{ -1,  0, 0 }, speed));
	input.BindKeyboardCommand(SDL_SCANCODE_D, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pQbertKnight, glm::vec3{ 1,  0, 0 }, speed));

	input.BindControllerCommand(0, dae::Controller::button::DpadUp, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pQbert, glm::vec3{ 0, -1, 0 }, doubleSpeed));
	input.BindControllerCommand(0, dae::Controller::button::DpadDown, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pQbert, glm::vec3{ 0,  1, 0 }, doubleSpeed));
	input.BindControllerCommand(0, dae::Controller::button::DpadLeft, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pQbert, glm::vec3{ -1,  0, 0 }, doubleSpeed));
	input.BindControllerCommand(0, dae::Controller::button::DpadRight, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pQbert, glm::vec3{ 1,  0, 0 }, doubleSpeed));


	
	//scene.Add(std::move(pivot));
	//scene.Add(std::move(parentChar));
	//scene.Add(std::move(childChar));
	scene.Add(std::move(cache));

}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
