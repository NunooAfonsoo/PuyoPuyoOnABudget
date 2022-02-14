#pragma once

#include "../../SDL2/include/SDL.h"
#include "../../SDL2_image-2.0.5/include/SDL_image.h"
#include "../../SDL2_ttf-2.0.15/include/SDL_ttf.h"

#include "GameWindow.h"
#include "Ball.h"

#include <iostream>
#include <map>

class AssetManager
{
	public:
		enum Font { Arial };
		static AssetManager* GetInstance();
		void ReleaseInstance();

		std::tuple<SDL_Texture*, Ball::Color> GetRandomEntityTexture();

		SDL_Texture* GetSquareTexture(Ball::Color ballColor);
		SDL_Texture* GetBallHelperTexture(Ball::Color ballColor);
		SDL_Texture*  GetBallTextureFromColor(Ball::Color ballColor);
		SDL_Color GetTextColor();
		SDL_Texture* GetFontTexture();
		TTF_Font* GetFont(Font font);

		SDL_Texture* GetEmptyCell();

		SDL_Texture* GetGridBackground();
		SDL_Texture* GetBallsBackground();
		SDL_Texture* GetMainMenuArrow();
		SDL_Texture* GetGameLogo();

	private:
		static AssetManager* instance;
		AssetManager();

		SDL_Texture* GetTexture(const char* textureFilePath);

		SDL_Texture* gridBackground;
		SDL_Texture* ballsBackground;
		SDL_Texture* mainMenuArrow;
		SDL_Texture* gameLogo;

		std::map<Ball::Color, SDL_Texture*> balls;
		std::map<Ball::Color, SDL_Texture*> ballHelpers;
		std::map<Ball::Color, SDL_Texture*> squares;

		SDL_Texture* emptyCell;

		SDL_Color textColor = { 255, 255, 255 };
		TTF_Font* arialFont;
		SDL_Texture* fontTexture;
};