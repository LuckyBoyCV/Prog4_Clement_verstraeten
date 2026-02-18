#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include <chrono>

namespace dae
{
	class Minigin final
	{
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

	private:

		static constexpr float Fixed_Time_Step = 1.f / 60.f; // 60 HZ  fixed update
		static constexpr float Max_Delta = 0.1f;    //spiral of death prevention

		bool m_quit{ false };
		float m_Lag{ 0.f };

		using Clock = std::chrono::high_resolution_clock;

		Clock::time_point m_LastTime = Clock::now();
	

	};
}