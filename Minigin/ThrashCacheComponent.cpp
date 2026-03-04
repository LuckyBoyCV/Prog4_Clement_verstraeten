#include "ThrashCacheComponent.h"
#include <imgui.h>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <string>

struct Transform
{
	float matrix[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
};

struct GameObject3D
{
	Transform* local{ nullptr };
	int ID{  };
};

struct GameObject3DAlt
{
	std::vector<Transform> transforms;
	std::vector<int> IDs;
};

dae::ThrashCacheComponent::ThrashCacheComponent(GameObject* pOwner)
	: Component(pOwner)
{
}


void dae::ThrashCacheComponent::Ex1()
{
	const int Array_Size = 1024;
	std::vector<int> arr(Array_Size);
	

	for (int stepsize = 1; stepsize <= Array_Size; stepsize *= 2)
	{
		auto start = std::chrono::high_resolution_clock::now();
		for (int repeat = 0; repeat < m_ex1Samples; repeat++)
		{
			for (int i = 0; i < Array_Size; i += stepsize)
			{
				arr[i] *= 2;
			}
		}
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		m_TimingsExc1.push_back(static_cast<float>(duration));
	}
}

void dae::ThrashCacheComponent::Ex2()
{
	const int Array_Size = 1024;
	std::vector<GameObject3D> Arr(Array_Size);

	for (int stepsize = 1; stepsize <= Array_Size; stepsize *= 2)
	{
		auto start = std::chrono::high_resolution_clock::now();
		for (int j = 0; j < m_ex2Samples; j++)
		{
			for (int i = 0; i < Array_Size; i += stepsize)
			{
				Arr[i].ID *= 2;
			}
		}
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		m_TimingsExc2.push_back(static_cast<float>(duration));
	}
}
void dae::ThrashCacheComponent::Ex2ALT()
{
	const int ARRAY_SIZE = 1024;
	GameObject3DAlt alt;
	alt.transforms.resize(ARRAY_SIZE);
	alt.IDs.resize(ARRAY_SIZE, 1);

	

	for (int stepsize = 1; stepsize <= ARRAY_SIZE; stepsize *= 2)
	{
		auto start = std::chrono::high_resolution_clock::now();
		for (int j = 0; j < m_ex2Samples; j++)
			for (int i = 0; i < (int)alt.IDs.size(); i += stepsize)
				alt.IDs[i] *= 2;
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		m_TimingsExc2Alt.push_back(static_cast<float>(duration));
	}
}
void dae::ThrashCacheComponent::Render()
{
	// Exercise 1
	ImGui::Begin("Exercise 1 - int buffer");

	ImGui::InputInt("# samples", &m_ex1Samples);
	if (m_ex1Samples < 1) m_ex1Samples = 1;

	if (ImGui::Button("Trash the cache"))
		Ex1();

	if (!m_TimingsExc1.empty())
	{
		float maxValue = *std::max_element(m_TimingsExc1.begin(), m_TimingsExc1.end());
		ImGui::PlotLines("##exc1", m_TimingsExc1.data(), (int)m_TimingsExc1.size(),
			0, nullptr, 0.0f, maxValue, ImVec2(400, 120));
	}

	ImGui::End();

	// Exercise 2
	ImGui::Begin("Exercise 2 - struct buffer");

	ImGui::InputInt("Sample Count", &m_ex2Samples);
	if (m_ex2Samples < 1) m_ex2Samples = 1;

	if (ImGui::Button("Trash the cache with GameObject3D"))
		Ex2();

	if (ImGui::Button("Trash the cache with GameObject3DAlt"))
		Ex2ALT();

	if (!m_TimingsExc2.empty())
	{
		ImGui::PlotLines("##exc2", m_TimingsExc2.data(), (int)m_TimingsExc2.size(),
			0, nullptr, 0.0f, *std::max_element(m_TimingsExc2.begin(), m_TimingsExc2.end()),
			ImVec2(400, 120));
	}

	if (!m_TimingsExc2Alt.empty())
	{
		ImGui::PlotLines("##exc2alt", m_TimingsExc2Alt.data(), (int)m_TimingsExc2Alt.size(),
			0, nullptr, 0.0f, *std::max_element(m_TimingsExc2Alt.begin(), m_TimingsExc2Alt.end()),
			ImVec2(400, 120));
	}

	ImGui::End();
}