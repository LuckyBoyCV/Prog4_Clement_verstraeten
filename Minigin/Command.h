#pragma once
namespace dae
{
	class GameObject;


	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
		void SetDeltaTime(float deltaTime) { m_deltaTime = deltaTime; }

	protected:
		float m_deltaTime{ 0.f };
	};


	class GameActorCommand: public Command
	{
	public:
		GameActorCommand(GameObject* pOwner) : m_pOwner(pOwner) {}
		virtual ~GameActorCommand() = default;
	private:
		GameObject* m_pOwner;

	protected:
		GameObject* GetOwner() const { return m_pOwner; }
	};
}