#pragma once
#include <string>
#include <memory>
#include <SDL3/SDL.h>
#include "Component.h"
#include "Texture2D.h"

namespace dae
{ 
class Font;


class TextComponent final : public Component
{
public:
	TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font);
	~TextComponent() override = default;

	void Update(float deltatime) override;	
	void Render() const override;

	void SetText(const std::string& text);
	void SetColor(SDL_Color color);	


	


private:
	void regerateTexture();
	bool m_NeedsUpdate{ true };
	SDL_Color m_Color{ 255, 255, 255, 255 };
	std::string m_Text;
	std::shared_ptr<Font> m_pFont;
	std::unique_ptr<Texture2D> m_pTexture{};


};
}