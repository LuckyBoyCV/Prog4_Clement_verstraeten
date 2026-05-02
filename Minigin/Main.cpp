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
#include "InputManager.h"
#include "MoveCommand.h"
#include "PlayerComponent.h"
#include "LivesDisplayComponent.h"
#include "ScoreDisplayComponent.h"
#include "DamageCommand.h"
#include "ScoreCommand.h"
#include "Controller.h"
#include "AchievementComponent.h"
#include "ServiceLocator.h"
#include "SDLSoundSystem.h"
#include "SoundCommand.h"
#include "../Q-bert/PyramidComponent.h"
#include "../Q-bert/QbertComponent.h"
#include "../Q-bert/JumpCommand.h"
#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto ss = std::make_unique<dae::SDLSoundSystem>();
	dae::sound_id jumpSound = ss->RegisterSound("Data/jump.wav");
	dae::ServiceLocator::RegisterSoundSystem(std::move(ss));


	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto& input = dae::InputManager::GetInstance();
	

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// background and logo
	auto Go = std::make_unique<dae::GameObject>();
	//Go->AddComponent<dae::RenderComponent>("background.png");
	//scene.Add(std::move(Go));

	//Go = std::make_unique<dae::GameObject>();
	//Go->SetPosition(358, 180);
	//Go->AddComponent<dae::RenderComponent>("logo.png");
	//scene.Add(std::move(Go));

	// title
	Go = std::make_unique<dae::GameObject>();
	Go->SetPosition(292, 15);
	Go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font)->SetColor({ 255, 255, 255, 255 });
	scene.Add(std::move(Go));

	// fps counter
	Go = std::make_unique<dae::GameObject>();
	Go->SetPosition(5, 5);
	Go->AddComponent<dae::TextComponent>("0.0 FPS", font)->SetColor({ 255, 255, 0, 255 });
	Go->AddComponent<dae::FPSComponent>();
	scene.Add(std::move(Go));

	//text
	Go = std::make_unique<dae::GameObject>();
	Go->SetPosition(5, 50);
	Go->AddComponent<dae::TextComponent>("Use the D-Pad to move Qbert, X to inflict damage, A and B to pick up pellets", smallFont)->SetColor({ 255, 200, 0, 255 });
	scene.Add(std::move(Go));

	Go = std::make_unique<dae::GameObject>();
	Go->SetPosition(5, 70);
	Go->AddComponent<dae::TextComponent>("Use WASD to move Qbert Knight, C to inflict damage, Z and X to pick up pellets", smallFont)->SetColor({ 200, 100, 255, 255 });
	scene.Add(std::move(Go));

	Go = std::make_unique<dae::GameObject>();
	Go->SetPosition(5, 250);
	Go->AddComponent<dae::TextComponent>("press spacebar to make jump sound", smallFont)->SetColor({ 255, 255, 255, 255 });
	scene.Add(std::move(Go));


	// player 1
	auto player1 = std::make_unique<dae::GameObject>();
	player1->SetPosition(300, 300);
	player1->AddComponent<dae::RenderComponent>("qbert.png")->SetScale(0.15f);
	auto* pPlayer1 = player1->AddComponent<dae::PlayerComponent>(3);

	auto livesGo1 = std::make_unique<dae::GameObject>();
	livesGo1->SetPosition(5, 95);
	livesGo1->AddComponent<dae::TextComponent>("# lives: 3", smallFont)->SetColor({ 255, 255, 255, 255 });
	auto* livesDisplay1 = livesGo1->AddComponent<dae::LivesDisplayComponent>();
	pPlayer1->m_subject.AddObserver(livesDisplay1);

	auto scoreGo1 = std::make_unique<dae::GameObject>();
	scoreGo1->SetPosition(5, 115);
	scoreGo1->AddComponent<dae::TextComponent>("Score: 0", smallFont)->SetColor({ 255, 255, 255, 255 });
	auto* scoreDisplay1 = scoreGo1->AddComponent<dae::ScoreDisplayComponent>();
	pPlayer1->m_subject.AddObserver(scoreDisplay1);
	 

	input.BindKeyboardCommand(SDL_SCANCODE_SPACE, dae::KeyState::Down, std::make_unique<dae::SoundCommand>(jumpSound, 1.0f));

	input.BindControllerCommand(0, dae::Controller::button::ButtonX, dae::KeyState::Down, std::make_unique<dae::DamageCommand>(player1.get()));
	input.BindControllerCommand(0, dae::Controller::button::ButtonA, dae::KeyState::Down, std::make_unique<dae::ScoreCommand>(player1.get(), 100));
	input.BindControllerCommand(0, dae::Controller::button::ButtonB, dae::KeyState::Down, std::make_unique<dae::ScoreCommand>(player1.get(), 100));
	input.BindControllerCommand(0, dae::Controller::button::DpadUp, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ 0, -1, 0 }, 40.f));
	input.BindControllerCommand(0, dae::Controller::button::DpadDown, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ 0,  1, 0 }, 40.f));
	input.BindControllerCommand(0, dae::Controller::button::DpadLeft, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ -1, 0, 0 }, 40.f));
	input.BindControllerCommand(0, dae::Controller::button::DpadRight, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ 1, 0, 0 }, 40.f));

	scene.Add(std::move(player1));
	scene.Add(std::move(livesGo1));
	scene.Add(std::move(scoreGo1));


	// player 2 
	auto player2 = std::make_unique<dae::GameObject>();
	player2->SetPosition(450, 300);
	player2->AddComponent<dae::RenderComponent>("qbert knight.png")->SetScale(0.15f);
	auto* pPlayer2 = player2->AddComponent<dae::PlayerComponent>(3);

	auto livesGo2 = std::make_unique<dae::GameObject>();
	livesGo2->SetPosition(5, 135);
	livesGo2->AddComponent<dae::TextComponent>("# lives: 3", smallFont)->SetColor({ 255, 255, 255, 255 });
	auto* livesDisplay2 = livesGo2->AddComponent<dae::LivesDisplayComponent>();
	pPlayer2->m_subject.AddObserver(livesDisplay2);

	auto scoreGo2 = std::make_unique<dae::GameObject>();
	scoreGo2->SetPosition(5, 155);
	scoreGo2->AddComponent<dae::TextComponent>("Score: 0", smallFont)->SetColor({ 255, 255, 255, 255 });
	auto* scoreDisplay2 = scoreGo2->AddComponent<dae::ScoreDisplayComponent>();
	pPlayer2->m_subject.AddObserver(scoreDisplay2);

	input.BindKeyboardCommand(SDL_SCANCODE_W, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ 0, -1, 0 }, 50.f));
	input.BindKeyboardCommand(SDL_SCANCODE_S, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ 0,  1, 0 }, 50.f));
	input.BindKeyboardCommand(SDL_SCANCODE_A, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ -1, 0, 0 }, 50.f));
	input.BindKeyboardCommand(SDL_SCANCODE_D, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ 1, 0, 0 }, 50.f));
	input.BindKeyboardCommand(SDL_SCANCODE_C, dae::KeyState::Down, std::make_unique<dae::DamageCommand>(player2.get()));
	input.BindKeyboardCommand(SDL_SCANCODE_Z, dae::KeyState::Down, std::make_unique<dae::ScoreCommand>(player2.get(), 100));
	input.BindKeyboardCommand(SDL_SCANCODE_X, dae::KeyState::Down, std::make_unique<dae::ScoreCommand>(player2.get(), 100));

	scene.Add(std::move(player2));
	scene.Add(std::move(livesGo2));
	scene.Add(std::move(scoreGo2));


	//achievement
	auto achievementGo = std::make_unique<dae::GameObject>();
	auto* achievement = achievementGo->AddComponent<dae::AchievementComponent>();
	pPlayer1->m_subject.AddObserver(achievement);
	pPlayer2->m_subject.AddObserver(achievement);
	scene.Add(std::move(achievementGo));

	//Pyramid
	auto pyramidGo = std::make_unique<dae::GameObject>();
	auto* pyramid = pyramidGo->AddComponent<qbert::PyramidComponent>();
	scene.Add(std::move(pyramidGo));

	auto qbertGo = std::make_unique<dae::GameObject>();
	auto* render = qbertGo->AddComponent<dae::RenderComponent>("sprites_Qbert.png");
	render->SetSourceRect(64, 0, 16, 22);  
	render->SetScale(3.f);
	qbertGo->AddComponent<qbert::QbertComponent>(pyramid, 0, 0);

	input.BindKeyboardCommand(SDL_SCANCODE_UP, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbertGo.get(), -1, 0));
	input.BindKeyboardCommand(SDL_SCANCODE_RIGHT, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbertGo.get(), +1, +1));
	input.BindKeyboardCommand(SDL_SCANCODE_DOWN, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbertGo.get(), +1, 0));
	input.BindKeyboardCommand(SDL_SCANCODE_LEFT, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbertGo.get(), -1, -1));
	
	scene.Add(std::move(qbertGo));
}

int main(int, char* []) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
	return 0;
}