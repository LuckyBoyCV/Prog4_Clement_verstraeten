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
		RenderComponent(GameObject* owner, const std::string& filename);
		~RenderComponent() override = default;

	void Render() const override;

	void SetTexture(const std::string& filename);
	

private:
	std::shared_ptr<Texture2D> m_texture{};
};
}