#pragma once
#include <memory>
#include "Component.h"
#include <string>
#include "Texture2D.h"

namespace dae
{

	class RenderComponent final : public Component
	{
	public:
		explicit RenderComponent(GameObject* owner);
		RenderComponent(GameObject* owner, const std::string& filename);
		~RenderComponent() override = default;

		void Render() const override;

		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D> texture);
		void SetScale(float scale) { m_scale = scale; }

	private:
		std::shared_ptr<Texture2D> m_pTexture{};
		float m_scale{ 1.0f };	
	};
}