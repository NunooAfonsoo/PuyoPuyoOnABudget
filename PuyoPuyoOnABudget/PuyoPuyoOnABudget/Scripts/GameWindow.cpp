#include "GameWindow.h"
#include "PuyoPuyo.h"
#include "Text.h"
#include "AssetManager.h"


GameWindow* GameWindow::instance = NULL;


GameWindow* GameWindow::GetInstance()
{
	if (!instance)
		instance = new GameWindow();
	return instance;
}


GameWindow::GameWindow()
{
	window = NULL;
	renderer = NULL;
}


GameWindow::~GameWindow()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	TTF_CloseFont(textFont);
	TTF_Quit();

	std::cout << "Game Closed!" << std::endl;
}


void GameWindow::ReleaseInstance()
{
	delete this;
}

/* Initializes SDL and TTF */
void GameWindow::InitializeGameWindow(const char* gameTitle, int xPos, int yPos, int windowWidth, int windowHeight)
{
	int flags = 0;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Game Started!" << std::endl;

		SDL_Window* window = SDL_CreateWindow(gameTitle, xPos, yPos, windowWidth, windowHeight, 0);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
		{
			std::cout << "Could't create Render. Error: " << SDL_GetError() << std::endl;
			return;
		}
		SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
	}


	if (TTF_Init() == -1)
	{
		std::cout << "Error initializing TTF" << std::endl;
	}
}


SDL_Renderer* GameWindow::GetRenderer()
{
	return renderer;
}


/* Renders entity texture */
void GameWindow::Render(SDL_Texture* texture, SDL_Rect dest)
{
	SDL_RenderCopy(renderer, texture, NULL, &dest);
}


/* Renders Text*/
void GameWindow::RenderText(Text* text, SDL_Rect dest)
{
	SDL_Surface* surface = TTF_RenderText_Blended(AssetManager::GetInstance()->GetFont(AssetManager::Font::Arial), text->GetText().c_str(), text->GetColor());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	int width, height;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_Rect dstRect = { dest.x, dest.y, dest.w, dest.h };
	SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}


void GameWindow::Display()
{
	SDL_RenderPresent(renderer);
}


void GameWindow::ClearGameWindow()
{
	SDL_RenderClear(renderer);
}