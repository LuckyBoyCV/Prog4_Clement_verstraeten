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


	auto pivot = std::make_unique<dae::GameObject>();
	pivot->SetLocalPostion(glm::vec3(512.f, 300.f, 0.f));

	auto parentChar = std::make_unique<dae::GameObject>();
	auto* qbert = parentChar->AddComponent<dae::RenderComponent>("qbert.png");
	qbert->SetScale(0.1f);
	// Radius 80 px, one full revolution every ~3 seconds counter-clockwise
	parentChar->AddComponent<dae::RotatorComponent>(10.f, 4.0f);
	parentChar->SetParent(pivot.get(), false);

	// Child character (another sprite, orbits its parent)
	auto childChar = std::make_unique<dae::GameObject>();
	auto* qbert_knight = childChar->AddComponent<dae::RenderComponent>("qbert knight.png");
	qbert_knight->SetScale(0.1f);
	// Radius 40 px, clockwise (negative speed) – opposite direction
	childChar->AddComponent<dae::RotatorComponent>(40.f, -3.0f);

	// Attach child BEFORE moving to scene so raw pointer stays valid.
	// keepWorldPosition = false: child starts at local (0,0) → on top of parent.
	childChar->SetParent(parentChar.get(), false);

	// The Scene owns both objects; order matters only for iteration.
	scene.Add(std::move(pivot));
	scene.Add(std::move(parentChar));
	scene.Add(std::move(childChar));

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
