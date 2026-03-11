#pragma once
#include "Component.h"
#include <vector>
#include <imgui.h>

namespace dae
{
	class ThrashCacheComponent final : public Component
	{
	public:
		ThrashCacheComponent(GameObject* pOwner);
		~ThrashCacheComponent() override = default;

		ThrashCacheComponent(const ThrashCacheComponent& other) = delete;
		ThrashCacheComponent(ThrashCacheComponent&& other) = delete;
		ThrashCacheComponent& operator=(const ThrashCacheComponent& other) = delete;
		ThrashCacheComponent& operator=(ThrashCacheComponent&& other) = delete;

		void Render() override;

	private:

		void Ex1();
		void Ex2();
		void Ex2ALT();

		static void PlotCombined(const std::vector<float>& dataA, const std::vector<float>& dataB,
			ImVec4 colorA, ImVec4 colorB, ImVec2 size);

		int m_ex1Samples{ 10 };
		std::vector<float> m_TimingsExc1;

		int m_ex2Samples{ 10 };
		std::vector<float> m_TimingsExc2;
		std::vector<float> m_TimingsExc2Alt;
	};
}