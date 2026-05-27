#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltatime)
{
	if (m_activeSceneIndex < m_scenes.size())
		m_scenes[m_activeSceneIndex]->Update(deltatime);
}

void dae::SceneManager::Render()
{
	if (m_activeSceneIndex < m_scenes.size())
		m_scenes[m_activeSceneIndex]->Render();
}

dae::Scene& dae::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());
	return *m_scenes.back();
}
