#include "GamePaused.h"

#include "../../SDL2/include/SDL.h"

#include "GameRunning.h"
#include "GameWindow.h"

#include "AssetManager.h"
#include <stdlib.h>


GamePaused::GamePaused()
{
	arrowPos = 0;

	game->ClearGamePausedEntities();

	/* Game Paused Entities */
	SDL_Rect destLogo = { 512, 64, 308, 177 };
	Entity* GameLogo = new Entity({ 0, 0, 0, 0 }, destLogo, AssetManager::GetInstance()->GetGameLogo(), Entity::EntityType::GamePaused);


	SDL_Rect destArrow = { 560, 322, 32, 32 };
	arrow = new Entity({ 0, 0, 0, 0 }, destArrow, AssetManager::GetInstance()->GetMainMenuArrow(), Entity::EntityType::GamePaused);

	SDL_Rect ReturnToGameTextDest = { 605, 324, 32, 32 };
	Text* ReturnToGameText = new Text({ 0, 0, 0, 0 }, ReturnToGameTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::GamePausedText);
	ReturnToGameText->UpdateText("Return To Game");


	SDL_Rect ExitGameTextDest = { 605, 390, 32, 32 };
	Text* ExitGameText = new Text({ 0, 0, 0, 0 }, ExitGameTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::GamePausedText);
	ExitGameText->UpdateText("Exit Game");
}


void GamePaused::Update()
{

	GameWindow::GetInstance()->ClearGameWindow();

	game->RenderGamePausedEntities();

	GameWindow::GetInstance()->Display();
}


void GamePaused::HandleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		game->SetGameIsRunning(false);
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_s: case SDLK_DOWN:
			printf("Key S Pressed\n");
			MoveArrow(-1);
			break;
		case SDLK_w: case SDLK_UP:
			printf("Key W Pressed\n");
			MoveArrow(1);
			break;
		case SDLK_RETURN: case SDLK_SPACE:
			printf("Key Enter Pressed\n");
			switch (arrowPos)
			{
			case 0:
				Timer::GetInstance()->SetTimeScale(Timer::GetInstance()->GetGameSpeed());
				game->SetGameState(new GameRunning(game->GetGameScore()));
				break;
			case 1:
				game->SetGameIsRunning(false);
				break;
			}
		}
		break;
	default:
		break;
	}
}


void GamePaused::MoveArrow(int direction)
{
	arrowPos += direction;
	arrowPos = abs(arrowPos) % 2; //Only 2 options at game menu so we devide by 2 to get 0 or 1, 0 start game, 1 exit

	SDL_Rect arrowDest = arrow->GetDESTRect();
	arrowDest.y = 322 + 66 * arrowPos;

	arrow->SetDESTRect(arrowDest);
}