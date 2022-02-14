#pragma once

#include "Entity.h"

#include "../../SDL2/include/SDL.h"

class Text : public Entity
{
	public:
		Text(SDL_Rect srcRec, SDL_Rect destRect, SDL_Texture* entTexture, EntityType type);

		void UpdateText(std::string updatedText);
		std::string GetText();

		void SetColor(SDL_Color newColor);
		SDL_Color GetColor();

	private:
		std::string text;
		SDL_Color color = { 255, 255, 255, 255 };
};

