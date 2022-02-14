#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "Text.h"
#include "Entity.h"

#include <iostream>


class GameWindow
{
public:
	static GameWindow* GetInstance();
	void ReleaseInstance();

	void InitializeGameWindow(const char* gameTitle, int xPos, int yPos, int windowWidth, int windowHeight);
	SDL_Renderer* GetRenderer();
	void Render(SDL_Texture* texture, SDL_Rect dest);
	void RenderText(Text* text, SDL_Rect dest);
	void Display();
	void ClearGameWindow();

private:
	GameWindow();
	~GameWindow();

	static GameWindow* instance;
	SDL_Window* window;
	SDL_Renderer* renderer;

	TTF_Font* textFont;
};

