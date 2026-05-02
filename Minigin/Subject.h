#pragma once
#include "../Q-bert/GameEvent.h"
#include <vector>
namespace dae
{

class Observer;
class GameObject;
	class Subject
	{
	public:
		Subject() = default;
		virtual ~Subject() = default;
		Subject(const Subject&) = delete;
		Subject(Subject&&) = delete;
		Subject& operator=(const Subject&) = delete;
		Subject& operator=(Subject&&) = delete;

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);
		void Notify(GameEvent event, GameObject* pOwner);


	private:
		std::vector<Observer*> m_observers{};



	};
}