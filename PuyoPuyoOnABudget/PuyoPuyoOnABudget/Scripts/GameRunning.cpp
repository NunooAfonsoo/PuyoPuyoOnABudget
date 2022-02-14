#include "GameRunning.h"

#include "../../SDL2/include/SDL.h"

#include "GameWindow.h"
#include "Timer.h"
#include "AssetManager.h"
#include "GamePaused.h"

#include <string>


GameRunning::GameRunning(int score)
{
	Timer::GetInstance()->SetTimeScale(Timer::GetInstance()->GetGameSpeed());

	/* Running Game entities*/
	SDL_Rect destBG = { 512, 130, 265, 529 };
	Entity* background = new Entity({ 0, 0, 0, 0 }, destBG, AssetManager::GetInstance()->GetGridBackground(), Entity::EntityType::Static);

	SDL_Rect ballDestBG1 = { 799, 163, 34, 67 };
	Entity* ballsBackground = new Entity({ 0, 0, 0, 0 }, ballDestBG1, AssetManager::GetInstance()->GetBallsBackground(), Entity::EntityType::Static);

	SDL_Rect ballDestBG2 = { 832, 196, 34, 67 };
	Entity* ballsBackground1 = new Entity({ 0, 0, 0, 0 }, ballDestBG2, AssetManager::GetInstance()->GetBallsBackground(), Entity::EntityType::Static);


	for (int i = 0; i < game->GetTextEntities().size(); i++)
	{
		if (game->GetTextEntities()[i] == game->GetScoreText())
		{
			game->EraseTextFromTextEntities(i);
			break;
		}
	}

	SDL_Rect scoreTextDest = { 799, 130, 0, 0 };
	Text* scoreText = new Text({ 0, 0, 0, 0 }, scoreTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::Text);
	scoreText->UpdateText("Score: " + std::to_string(score));
	game->SetScoreText(scoreText);


	SDL_Rect gameOverTextDest = { 496, 64, 0, 0 };
	Text* gameOverText = new Text({ 0, 0, 0, 0 }, gameOverTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::Text);
	gameOverText->UpdateText("");
	game->SetGameOverText(gameOverText);


	for (int i = 0; i < game->GetTextEntities().size(); i++)
	{
		if (game->GetTextEntities()[i] == game->GetPauseGameText())
		{
			game->EraseTextFromTextEntities(i);
			break;
		}
	}

	SDL_Rect PauseTextDest = { 555, 64, 0, 0 };
	Text* PauseText = new Text({ 0, 0, 0, 0 }, PauseTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::Text);
	PauseText->UpdateText("");
	game->SetPauseGameText(PauseText);


	SDL_Rect ControlsTextDest = { 20, 130, 64, 32 };
	Text* ControlsText = new Text({ 0, 0, 0, 0 }, ControlsTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::Text);
	ControlsText->UpdateText("Controls:");


	SDL_Rect wKeyTextDest = { 50, 163, 64, 32 };
	Text* wKeyText = new Text({ 0, 0, 0, 0 }, wKeyTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::Text);
	wKeyText->UpdateText("W/Arrow Up: Teleport Pieces to Bottom");


	SDL_Rect aKeyTextDest = { 50, 196, 64, 32 };
	Text* aKeyText = new Text({ 0, 0, 0, 0 }, aKeyTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::Text);
	aKeyText->UpdateText("A/Arrow Left: Move Left");


	SDL_Rect sKeyTextDest = { 50, 229, 64, 32 };
	Text* sKeyText = new Text({ 0, 0, 0, 0 }, sKeyTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::Text);
	sKeyText->UpdateText("S/Arrow Down: Move Down");


	SDL_Rect dKeyTextDest = { 50, 262, 64, 32 };
	Text* dKeyText = new Text({ 0, 0, 0, 0 }, dKeyTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::Text);
	dKeyText->UpdateText("D/Arrow Right: Move Right");


	SDL_Rect qKeyTextDest = { 50, 295, 64, 32 };
	Text* qKeyText = new Text({ 0, 0, 0, 0 }, qKeyTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::Text);
	qKeyText->UpdateText("Q: Rotate Left");


	SDL_Rect eKeyTextDest = { 50, 328, 64, 32 };
	Text* eKeyText = new Text({ 0, 0, 0, 0 }, eKeyTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::Text);
	eKeyText->UpdateText("E: Rotate Right");

	SDL_Rect spaceKeyTextDest = { 50, 361, 64, 32 };
	Text* spaceKeyText = new Text({ 0, 0, 0, 0 }, spaceKeyTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::Text);
	spaceKeyText->UpdateText("Space: Pause/Unpause Game");

	SDL_Rect escKeyTextDest = { 50, 394, 64, 32 };
	Text* escKeyText = new Text({ 0, 0, 0, 0 }, escKeyTextDest, AssetManager::GetInstance()->GetFontTexture(), Entity::EntityType::Text);
	escKeyText->UpdateText("Esc: Pause Menu");
}


void GameRunning::Update()
{
	GameWindow::GetInstance()->ClearGameWindow();

	game->RenderAllEntities();

	GameWindow::GetInstance()->Display();

	Timer::GetInstance()->Update();
}


void GameRunning::HandleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	float tScale = Timer::GetInstance()->GetTimeScale();
	bool keyUp = false;
	switch (event.type)
	{
	case SDL_QUIT:
		game->SetGameIsRunning(false);
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{

		case SDLK_d: case SDLK_RIGHT:
			printf("Key D Pressed\n");
			if (game->GetBallSetLeaders()[0] != nullptr && tScale != 0)
			{
				game->GetBallSetLeaders()[0]->MoveBallSetRight();
			}
			break;
		case SDLK_a: case SDLK_LEFT:
			if (game->GetBallSetLeaders()[0] != nullptr && tScale != 0)
			{
				printf("Key A Pressed\n");
				game->GetBallSetLeaders()[0]->MoveBallSetLeft();
			}
			break;
		case SDLK_e:
			if (game->GetBallSetLeaders()[0] != nullptr && tScale != 0)
			{
				printf("Key E Pressed\n");
				game->GetBallSetLeaders()[0]->RotateBallSetRight();
			}
			break;
		case SDLK_q:
			if (game->GetBallSetLeaders()[0] != nullptr && tScale != 0)
			{
				printf("Key Q Pressed\n");
				game->GetBallSetLeaders()[0]->RotateBallSetLeft();
			}
			break;
		case SDLK_s: case SDLK_DOWN:
			if (game->GetBallSetLeaders()[0] != nullptr && tScale != 0)
			{
				printf("Key S Pressed\n");
				game->GetBallSetLeaders()[0]->MoveBallSetDown();
			}
			break;
		case SDLK_w: case SDLK_UP:
			if (game->GetBallSetLeaders()[0] != nullptr && tScale != 0)
			{
				printf("Key W Pressed\n");
				game->GetBallSetLeaders()[0]->SimulateBallSetEntireMovement();
			}
			break;
		case SDLK_SPACE:
			if (Timer::GetInstance()->GetGameSpeed() != 0)
			{
				printf("Key SPACE Pressed\n");
				if (tScale == 0)
				{
					Timer::GetInstance()->SetTimeScale(Timer::GetInstance()->GetGameSpeed());
					game->GetPauseGameText()->UpdateText("");
				}
				else
				{
					Timer::GetInstance()->SetTimeScale(0);
					game->GetPauseGameText()->UpdateText("Game Paused");
				}
				game->GameGrid->PrintGrid();
			}
			break;
		case SDLK_ESCAPE:
			Timer::GetInstance()->SetTimeScale(0);
			game->SetGameState(new GamePaused);
			break;
		}
		break;
	default:
		break;
	}
}
