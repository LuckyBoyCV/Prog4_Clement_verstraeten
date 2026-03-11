#include "ThrashCacheComponent.h"
#include <imgui.h>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm>

struct Transform
{
	float matrix[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
};

struct GameObject3D
{
	Transform transform;
	int ID{};
};

// Alt: no inline transform, only the ID is stored
// iterating only loads 4 bytes per element instead of 68
struct GameObject3DAlt
{
	int ID{};
};

template<typename Fn>
static std::vector<float> RunCacheTest(Fn loopFn, int numSamples)
{
	using Clock = std::chrono::high_resolution_clock;

	numSamples = std::max(3, numSamples);

	std::vector<float> timings;
	timings.reserve(11);

	for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
	{
		std::vector<long long> samples;
		samples.reserve(numSamples);

		for (int s = 0; s < numSamples; ++s)
		{
			auto start = Clock::now();
			loopFn(stepSize);
			auto end = Clock::now();
			samples.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		}

		std::sort(samples.begin(), samples.end());
		samples.erase(samples.begin());
		samples.pop_back();

		const long long avg = std::accumulate(samples.begin(), samples.end(), 0LL) / static_cast<long long>(samples.size());
		timings.push_back(static_cast<float>(avg));
	}

	return timings;
}

void dae::ThrashCacheComponent::PlotCombined(const std::vector<float>& dataA, const std::vector<float>& dataB,
	ImVec4 colorA, ImVec4 colorB, ImVec2 size)
{
	const float maxA = *std::max_element(dataA.begin(), dataA.end());
	const float maxB = *std::max_element(dataB.begin(), dataB.end());
	const float sharedMax = std::max(maxA, maxB);

	const ImVec2 canvasPos = ImGui::GetCursorScreenPos();
	ImGui::InvisibleButton("##combinedCanvas", size);
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	drawList->AddRectFilled(canvasPos,
		ImVec2(canvasPos.x + size.x, canvasPos.y + size.y),
		IM_COL32(30, 30, 30, 255));

	auto drawLine = [&](const std::vector<float>& data, ImVec4 col)
		{
			const int count = (int)data.size();
			const ImU32 imCol = ImGui::ColorConvertFloat4ToU32(col);
			for (int i = 1; i < count; ++i)
			{
				float x0 = canvasPos.x + (float)(i - 1) / (float)(count - 1) * size.x;
				float y0 = canvasPos.y + size.y - (data[i - 1] / sharedMax) * size.y;
				float x1 = canvasPos.x + (float)i / (float)(count - 1) * size.x;
				float y1 = canvasPos.y + size.y - (data[i] / sharedMax) * size.y;
				drawList->AddLine(ImVec2(x0, y0), ImVec2(x1, y1), imCol, 2.f);
			}
		};

	drawLine(dataA, colorA);
	drawLine(dataB, colorB);
}

dae::ThrashCacheComponent::ThrashCacheComponent(GameObject* pOwner)
	: Component(pOwner)
{
}

void dae::ThrashCacheComponent::Ex1()
{
	constexpr int arraySize = 1 << 25;
	int* arr = new int[arraySize]();

	m_TimingsExc1 = RunCacheTest([&](int stepSize)
		{
			for (int i = 0; i < arraySize; i += stepSize)
				arr[i] *= 2;
		}, m_ex1Samples);

	delete[] arr;
}

void dae::ThrashCacheComponent::Ex2()
{
	constexpr int arraySize = 1 << 22;
	GameObject3D* arr = new GameObject3D[arraySize]();

	m_TimingsExc2 = RunCacheTest([&](int stepSize)
		{
			for (int i = 0; i < arraySize; i += stepSize)
				arr[i].ID *= 2;
		}, m_ex2Samples);

	delete[] arr;
}

void dae::ThrashCacheComponent::Ex2ALT()
{
	constexpr int arraySize = 1 << 22;
	GameObject3DAlt* arr = new GameObject3DAlt[arraySize]();

	m_TimingsExc2Alt = RunCacheTest([&](int stepSize)
		{
			for (int i = 0; i < arraySize; i += stepSize)
				arr[i].ID *= 2;
		}, m_ex2Samples);

	delete[] arr;
}

void dae::ThrashCacheComponent::Render()
{
	// Exercise 1
	ImGui::Begin("Exercise 1 - int buffer");

	ImGui::InputInt("# samples##ex1", &m_ex1Samples);
	if (m_ex1Samples < 3) m_ex1Samples = 3;

	if (ImGui::Button("Trash the cache##ex1"))
		Ex1();

	if (!m_TimingsExc1.empty())
	{
		const float maxVal = *std::max_element(m_TimingsExc1.begin(), m_TimingsExc1.end());
		ImGui::PlotLines("##exc1", m_TimingsExc1.data(), (int)m_TimingsExc1.size(),
			0, nullptr, 0.0f, maxVal, ImVec2(400, 120));
	}

	ImGui::End();

	// Exercise 2
	ImGui::Begin("Exercise 2 - GameObject3D");

	ImGui::InputInt("# samples##ex2", &m_ex2Samples);
	if (m_ex2Samples < 3) m_ex2Samples = 3;

	if (ImGui::Button("Trash the cache with GameObject3D"))
		Ex2();

	if (ImGui::Button("Trash the cache with GameObject3DAlt"))
		Ex2ALT();

	if (!m_TimingsExc2.empty())
	{
		const float maxVal = *std::max_element(m_TimingsExc2.begin(), m_TimingsExc2.end());
		ImGui::Text("GameObject3D:");
		ImGui::PlotLines("##exc2", m_TimingsExc2.data(), (int)m_TimingsExc2.size(),
			0, nullptr, 0.0f, maxVal, ImVec2(400, 120));
	}

	if (!m_TimingsExc2Alt.empty())
	{
		const float maxVal = *std::max_element(m_TimingsExc2Alt.begin(), m_TimingsExc2Alt.end());
		ImGui::Text("GameObject3DAlt:");
		ImGui::PlotLines("##exc2alt", m_TimingsExc2Alt.data(), (int)m_TimingsExc2Alt.size(),
			0, nullptr, 0.0f, maxVal, ImVec2(400, 120));
	}

	// Exercise 3 - combined graph
	if (!m_TimingsExc2.empty() && !m_TimingsExc2Alt.empty())
	{
		ImGui::Separator();
		ImGui::Text("Combined:");
		PlotCombined(
			m_TimingsExc2, m_TimingsExc2Alt,
			ImVec4(0.f, 1.f, 0.f, 1.f),
			ImVec4(0.f, 0.5f, 1.f, 1.f),
			ImVec2(400, 120));
	}

	ImGui::End();
}