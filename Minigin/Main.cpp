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
#include "InputManager.h"
#include "../Q-bert/PyramidComponent.h"
#include "../Q-bert/QbertComponent.h"
#include "../Q-bert/JumpCommand.h"
#include "../Q-bert/CoilyComponent.h"
#include "../Q-bert/RedBallComponent.h"
#include "../Q-bert/GameStateComponent.h"
#include "../Q-bert/LevelDisplayComponent.h"
#include "../Q-bert/RoundDisplayComponent.h"
#include "../Q-bert/LivesDisplayComponent.h"
#include "../Q-bert/ScoreDisplayComponent.h"
#include "../Q-bert/ChangeToComponent.h"
#include "../Q-bert/SlickSamComponent.h"
#include "../Q-bert/UggWrongwayComponent.h"
#include "../Q-bert/mainMenuComponent.h"
#include "../Q-bert/MenuCommand.h"
#include "../Q-bert/SkipLevelCommand.h"
#include "../Q-bert/coilyJumpCommand.h"
#include "../Q-bert/gameMode.h"
#include "../Q-bert/Enemy.h"
#include "../Q-bert/enemySpawnerComponent.h"
#include "../Q-bert/levelConfig.h"

#include <filesystem>
#include <cstdlib>
#include <ctime>
#include <utility>
namespace fs = std::filesystem;

static void loadGame(qbert::gameMode mode);

static void loadMenu()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto largeFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 52);
	auto menuFont  = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	// Title
	auto titleGo = std::make_unique<dae::GameObject>();
	titleGo->SetPosition(350.f, 70.f);
	titleGo->AddComponent<dae::TextComponent>("Q*BERT", largeFont)->SetColor({ 255, 165, 0, 255 });
	scene.Add(std::move(titleGo));

	
	// Menu options
	constexpr float menuX = 380.f;
	std::vector<dae::TextComponent*> menuModes;

	auto mode1Go = std::make_unique<dae::GameObject>();
	mode1Go->SetPosition(menuX, 230.f);
	menuModes.push_back(mode1Go->AddComponent<dae::TextComponent>("Single Player", menuFont));
	scene.Add(std::move(mode1Go));

	auto mode2Go = std::make_unique<dae::GameObject>();
	mode2Go->SetPosition(menuX, 295.f);
	menuModes.push_back(mode2Go->AddComponent<dae::TextComponent>("Co-op", menuFont));
	scene.Add(std::move(mode2Go));

	auto mode3Go = std::make_unique<dae::GameObject>();
	mode3Go->SetPosition(menuX, 360.f);
	menuModes.push_back(mode3Go->AddComponent<dae::TextComponent>("Versus", menuFont));
	scene.Add(std::move(mode3Go));

	
	// Menu controller
	auto menuGo = std::make_unique<dae::GameObject>();
	auto* menuComp = menuGo->AddComponent<qbert::MainMenuComponent>(menuModes, loadGame);
	scene.Add(std::move(menuGo));

	auto& input = dae::InputManager::GetInstance();
	input.ClearAllBindings();
	input.BindKeyboardCommand(SDL_SCANCODE_UP, dae::KeyState::Down, std::make_unique<qbert::MenuNavigateCommand>(menuComp, -1));
	input.BindKeyboardCommand(SDL_SCANCODE_DOWN, dae::KeyState::Down, std::make_unique<qbert::MenuNavigateCommand>(menuComp, +1));
	input.BindKeyboardCommand(SDL_SCANCODE_RETURN, dae::KeyState::Down, std::make_unique<qbert::MenuConfirmCommand>(menuComp));

	// Controller 0 can also drive the menu (D-pad to move, A to confirm)
	input.BindControllerCommand(0, dae::Controller::button::DpadUp, dae::KeyState::Down, std::make_unique<qbert::MenuNavigateCommand>(menuComp, -1));
	input.BindControllerCommand(0, dae::Controller::button::DpadDown, dae::KeyState::Down, std::make_unique<qbert::MenuNavigateCommand>(menuComp, +1));
	input.BindControllerCommand(0, dae::Controller::button::ButtonA, dae::KeyState::Down, std::make_unique<qbert::MenuConfirmCommand>(menuComp));
}

static void loadGame(qbert::gameMode mode)
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& input = dae::InputManager::GetInstance();
	input.ClearAllBindings();

	auto& scene = sceneManager.CreateScene();
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// Pyramid
	auto pyramidGo = std::make_unique<dae::GameObject>();
	auto* pyramid = pyramidGo->AddComponent<qbert::PyramidComponent>();
	scene.Add(std::move(pyramidGo));

	qbert::QbertComponent* qbert1 = nullptr;
	qbert::QbertComponent* qbert2 = nullptr;

	if (mode == qbert::gameMode::singlePlayer || mode == qbert::gameMode::Versus)
	{
		auto qbertGo = std::make_unique<dae::GameObject>();
		auto* render = qbertGo->AddComponent<dae::RenderComponent>("sprites_Qbert.png");
		render->SetSourceRect(64, 0, 16, 22);
		render->SetScale(3.f);
		qbertGo->AddComponent<qbert::QbertComponent>(pyramid, 0, 0);
		qbert1 = qbertGo->GetComponent<qbert::QbertComponent>();

		input.BindKeyboardCommand(SDL_SCANCODE_UP, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbertGo.get(), -1,  0));
		input.BindKeyboardCommand(SDL_SCANCODE_RIGHT, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbertGo.get(), +1, +1));
		input.BindKeyboardCommand(SDL_SCANCODE_DOWN, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbertGo.get(), +1,  0));
		input.BindKeyboardCommand(SDL_SCANCODE_LEFT, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbertGo.get(), -1, -1));

		// Player 1 controller (controller 0, D-pad)
		input.BindControllerCommand(0, dae::Controller::button::DpadUp, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbertGo.get(), -1,  0));
		input.BindControllerCommand(0, dae::Controller::button::DpadRight, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbertGo.get(), +1, +1));
		input.BindControllerCommand(0, dae::Controller::button::DpadDown, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbertGo.get(), +1,  0));
		input.BindControllerCommand(0, dae::Controller::button::DpadLeft, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbertGo.get(), -1, -1));

		scene.Add(std::move(qbertGo));
	}
	else if (mode == qbert::gameMode::Coop)
	{
		// Player 1 starts bottom left corner
		auto qbert1Go = std::make_unique<dae::GameObject>();
		auto* render1 = qbert1Go->AddComponent<dae::RenderComponent>("sprites_Qbert.png");
		render1->SetSourceRect(64, 0, 16, 22);
		render1->SetScale(3.f);
		qbert1Go->AddComponent<qbert::QbertComponent>(pyramid, 6, 0);
		qbert1 = qbert1Go->GetComponent<qbert::QbertComponent>();

		input.BindKeyboardCommand(SDL_SCANCODE_UP, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert1Go.get(), -1,  0));
		input.BindKeyboardCommand(SDL_SCANCODE_RIGHT, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert1Go.get(), +1, +1));
		input.BindKeyboardCommand(SDL_SCANCODE_DOWN, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert1Go.get(), +1,  0));
		input.BindKeyboardCommand(SDL_SCANCODE_LEFT, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert1Go.get(), -1, -1));

		// Player 1 controller (controller 0, D-pad)
		input.BindControllerCommand(0, dae::Controller::button::DpadUp, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert1Go.get(), -1,  0));
		input.BindControllerCommand(0, dae::Controller::button::DpadRight, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert1Go.get(), +1, +1));
		input.BindControllerCommand(0, dae::Controller::button::DpadDown, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert1Go.get(), +1,  0));
		input.BindControllerCommand(0, dae::Controller::button::DpadLeft, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert1Go.get(), -1, -1));

		// Player 2 starts bottom right corner
		auto qbert2Go = std::make_unique<dae::GameObject>();
		auto* render2 = qbert2Go->AddComponent<dae::RenderComponent>("sprites_Qbert.png");
		render2->SetSourceRect(64, 0, 16, 22);
		render2->SetScale(3.f);
		qbert2Go->AddComponent<qbert::QbertComponent>(pyramid, 6, 6);
		qbert2 = qbert2Go->GetComponent<qbert::QbertComponent>();

		input.BindKeyboardCommand(SDL_SCANCODE_W, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert2Go.get(), -1,  0));
		input.BindKeyboardCommand(SDL_SCANCODE_D, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert2Go.get(), +1, +1));
		input.BindKeyboardCommand(SDL_SCANCODE_S, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert2Go.get(), +1,  0));
		input.BindKeyboardCommand(SDL_SCANCODE_A, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert2Go.get(), -1, -1));

		// Player 2 controller (controller 1, D-pad)
		input.BindControllerCommand(1, dae::Controller::button::DpadUp, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert2Go.get(), -1,  0));
		input.BindControllerCommand(1, dae::Controller::button::DpadRight, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert2Go.get(), +1, +1));
		input.BindControllerCommand(1, dae::Controller::button::DpadDown, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert2Go.get(), +1,  0));
		input.BindControllerCommand(1, dae::Controller::button::DpadLeft, dae::KeyState::Down, std::make_unique<qbert::JumpCommand>(qbert2Go.get(), -1, -1));

		scene.Add(std::move(qbert1Go));
		scene.Add(std::move(qbert2Go));
	}

	// Coily
	auto coilyGo = std::make_unique<dae::GameObject>();
	coilyGo->AddComponent<dae::RenderComponent>("sprites_Qbert.png")->SetScale(2.f);
	auto* coily = coilyGo->AddComponent<qbert::CoilyComponent>(pyramid, qbert1);

	if (mode == qbert::gameMode::Versus)
	{
		coily->setPlayerControlled(true);
		// Player 2 controls Coily with WASD once it hatches
		input.BindKeyboardCommand(SDL_SCANCODE_W, dae::KeyState::Down, std::make_unique<qbert::coilyJumpCommand>(coilyGo.get(), -1,  0));
		input.BindKeyboardCommand(SDL_SCANCODE_D, dae::KeyState::Down, std::make_unique<qbert::coilyJumpCommand>(coilyGo.get(), +1, +1));
		input.BindKeyboardCommand(SDL_SCANCODE_S, dae::KeyState::Down, std::make_unique<qbert::coilyJumpCommand>(coilyGo.get(), +1,  0));
		input.BindKeyboardCommand(SDL_SCANCODE_A, dae::KeyState::Down, std::make_unique<qbert::coilyJumpCommand>(coilyGo.get(), -1, -1));

		// Player 2 controls Coily with controller 1 (D-pad)
		input.BindControllerCommand(1, dae::Controller::button::DpadUp, dae::KeyState::Down, std::make_unique<qbert::coilyJumpCommand>(coilyGo.get(), -1,  0));
		input.BindControllerCommand(1, dae::Controller::button::DpadRight, dae::KeyState::Down, std::make_unique<qbert::coilyJumpCommand>(coilyGo.get(), +1, +1));
		input.BindControllerCommand(1, dae::Controller::button::DpadDown, dae::KeyState::Down, std::make_unique<qbert::coilyJumpCommand>(coilyGo.get(), +1,  0));
		input.BindControllerCommand(1, dae::Controller::button::DpadLeft, dae::KeyState::Down, std::make_unique<qbert::coilyJumpCommand>(coilyGo.get(), -1, -1));
	}
	scene.Add(std::move(coilyGo));

	// Red ball
	auto redBallGo = std::make_unique<dae::GameObject>();
	redBallGo->AddComponent<dae::RenderComponent>("sprites_Qbert.png")->SetScale(2.f);
	auto* redBall = redBallGo->AddComponent<qbert::redBallComponent>(pyramid, qbert1);
	scene.Add(std::move(redBallGo));

	// Slick
	auto slickGo = std::make_unique<dae::GameObject>();
	auto* slickRender = slickGo->AddComponent<dae::RenderComponent>("sprites_Qbert.png");
	slickRender->SetSourceRect(82, 142, 12, 18);
	slickRender->SetScale(2.5f);
	auto* slick = slickGo->AddComponent<qbert::SlickSamComponent>(pyramid, qbert1);
	scene.Add(std::move(slickGo));

	// Sam
	auto samGo = std::make_unique<dae::GameObject>();
	auto* samRender = samGo->AddComponent<dae::RenderComponent>("sprites_Qbert.png");
	samRender->SetSourceRect(82, 127, 12, 18);
	samRender->SetScale(2.5f);
	auto* sam = samGo->AddComponent<qbert::SlickSamComponent>(pyramid, qbert1);
	scene.Add(std::move(samGo));

	// Ugg climbs the right edge of the pyramid upward, lethal to Q*bert on contact
	auto uggGo = std::make_unique<dae::GameObject>();
	auto* uggRender = uggGo->AddComponent<dae::RenderComponent>("sprites_Qbert.png");
	uggRender->SetSourceRect(82, 97, 12, 18);   // estimated purple Ugg frame, tweak to pixel fit
	uggRender->SetScale(2.5f);
	auto* ugg = uggGo->AddComponent<qbert::UggWrongwayComponent>(pyramid, qbert1, qbert::UggWrongwaySide::right);
	scene.Add(std::move(uggGo));

	// Wrongway climbs the left edge of the pyramid upward, lethal to Q*bert on contact
	auto wrongwayGo = std::make_unique<dae::GameObject>();
	auto* wrongwayRender = wrongwayGo->AddComponent<dae::RenderComponent>("sprites_Qbert.png");
	wrongwayRender->SetSourceRect(82, 112, 12, 18); // estimated purple Wrongway frame, tweak to pixel fit
	wrongwayRender->SetScale(2.5f);
	auto* wrongway = wrongwayGo->AddComponent<qbert::UggWrongwayComponent>(pyramid, qbert1, qbert::UggWrongwaySide::left);
	scene.Add(std::move(wrongwayGo));

	// Wire up observers
	qbert1->m_subject.AddObserver(coily);
	qbert1->m_subject.AddObserver(redBall);
	qbert1->m_subject.AddObserver(slick);
	qbert1->m_subject.AddObserver(sam);
	qbert1->m_subject.AddObserver(ugg);
	qbert1->m_subject.AddObserver(wrongway);

	if (qbert2)
	{
		qbert2->m_subject.AddObserver(coily);
		qbert2->m_subject.AddObserver(redBall);
		qbert2->m_subject.AddObserver(slick);
		qbert2->m_subject.AddObserver(sam);
		qbert2->m_subject.AddObserver(ugg);
		qbert2->m_subject.AddObserver(wrongway);
	}

	// Player HUD (top-left): score 
	// Each display observes its own Q*bert and refreshes on the matching event;
	// initial text shows the starting values.
	auto scoreFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 28);

	auto score1Go = std::make_unique<dae::GameObject>();
	score1Go->SetPosition(40.f, 15.f);
	score1Go->AddComponent<dae::TextComponent>(std::to_string(qbert1->getScore()), scoreFont)->SetColor({ 255, 165, 0, 255 });
	qbert1->m_subject.AddObserver(score1Go->AddComponent<qbert::ScoreDisplayComponent>());
	scene.Add(std::move(score1Go));

	auto lives1Go = std::make_unique<dae::GameObject>();
	lives1Go->SetPosition(40.f, 50.f);
	lives1Go->AddComponent<dae::TextComponent>("Lives: " + std::to_string(qbert1->getLives()), smallFont)->SetColor({ 255, 255, 255, 255 });
	qbert1->m_subject.AddObserver(lives1Go->AddComponent<qbert::LivesDisplayComponent>());
	scene.Add(std::move(lives1Go));

	if (qbert2)
	{
		auto score2Go = std::make_unique<dae::GameObject>();
		score2Go->SetPosition(40.f, 95.f);
		score2Go->AddComponent<dae::TextComponent>(std::to_string(qbert2->getScore()), scoreFont)->SetColor({ 255, 165, 0, 255 });
		qbert2->m_subject.AddObserver(score2Go->AddComponent<qbert::ScoreDisplayComponent>());
		scene.Add(std::move(score2Go));

		auto lives2Go = std::make_unique<dae::GameObject>();
		lives2Go->SetPosition(40.f, 130.f);
		lives2Go->AddComponent<dae::TextComponent>("Lives: " + std::to_string(qbert2->getLives()), smallFont)->SetColor({ 255, 255, 255, 255 });
		qbert2->m_subject.AddObserver(lives2Go->AddComponent<qbert::LivesDisplayComponent>());
		scene.Add(std::move(lives2Go));
	}

	// CHANGE TO:indicator: plain-text label + the target swatch sprite, which
	// follows the pyramid's colour set
	auto changeToGo = std::make_unique<dae::GameObject>();
	changeToGo->SetPosition(744.f, 150.f);
	changeToGo->AddComponent<dae::TextComponent>("CHANGE TO:", smallFont)->SetColor({ 255, 255, 255, 255 });
	changeToGo->AddComponent<qbert::ChangeToComponent>(pyramid);
	scene.Add(std::move(changeToGo));

	// Game state UI
	auto gameStateGo = std::make_unique<dae::GameObject>();
	auto* gameState = gameStateGo->AddComponent<qbert::GameStateComponent>();

	// Round progression: completing the pyramid advances the round and resets things
	gameState->setPyramid(pyramid);
	gameState->addQbert(qbert1);
	gameState->addQbert(qbert2);

	// Levels are data-driven: read the table from Data/levels.json  and hand it to the game state. Must come
	// after setPyramid so level 1's flip rule/colour can be applied to the pyramid now.
	auto levels = qbert::loadLevelConfig(dae::ResourceManager::GetInstance().GetDataPath() / "levels.json");
	gameState->setConfig(std::move(levels));

	// F1 skips straight to the next level (debug/demo shortcut)
	input.BindKeyboardCommand(SDL_SCANCODE_F1, dae::KeyState::Down, std::make_unique<qbert::SkipLevelCommand>(gameState));

	// Enemy spawner:  which enemies are active per round and enforces the cap
	auto spawnerGo = std::make_unique<dae::GameObject>();
	auto* spawner = spawnerGo->AddComponent<qbert::EnemySpawnerComponent>(gameState, coily, redBall, slick, sam, ugg, wrongway);
	gameState->m_subject.AddObserver(spawner);
	slick->m_subject.AddObserver(spawner); // EnemyDied frees the slot
	sam->m_subject.AddObserver(spawner);
	scene.Add(std::move(spawnerGo));

	auto levelLabelGo = std::make_unique<dae::GameObject>();
	levelLabelGo->SetPosition(840.f, 50.f);

	auto levelNumberGo = std::make_unique<dae::GameObject>();
	levelNumberGo->SetPosition(850.f, 50.f);
	levelNumberGo->AddComponent<dae::TextComponent>(std::to_string(gameState->getLevel()), smallFont)->SetColor({ 255, 255, 255, 255 });

	auto* levelDisplay = levelLabelGo->AddComponent<qbert::LevelDisplayComponent>(levelNumberGo.get());
	gameState->m_subject.AddObserver(levelDisplay);

	auto roundLabelGo = std::make_unique<dae::GameObject>();
	roundLabelGo->SetPosition(840.f, 100.f);

	auto roundNumberGo = std::make_unique<dae::GameObject>();
	roundNumberGo->SetPosition(850.f, 115.f);
	roundNumberGo->AddComponent<dae::TextComponent>(std::to_string(gameState->getRound()), smallFont)->SetColor({ 255, 255, 255, 255 });

	auto* roundDisplay = roundLabelGo->AddComponent<qbert::RoundDisplayComponent>(roundNumberGo.get());
	gameState->m_subject.AddObserver(roundDisplay);

	scene.Add(std::move(gameStateGo));
	scene.Add(std::move(levelLabelGo));
	scene.Add(std::move(levelNumberGo));
	scene.Add(std::move(roundLabelGo));
	scene.Add(std::move(roundNumberGo));

	// Switch to game scene (menu is scene 0, game is scene 1)
	sceneManager.SetActiveScene(1);
}

static void load()
{
	loadMenu();
}

int main(int, char* [])
{
	srand(static_cast<unsigned int>(time(nullptr)));
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
