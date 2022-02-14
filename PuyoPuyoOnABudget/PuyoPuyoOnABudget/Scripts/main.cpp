#include "../../SDL2/include/SDL.h"
#include "PuyoPuyo.h"
#include "GameWindow.h"
#include "AssetManager.h"
#include "Ball.h"
#include "Timer.h"
#include "Entity.h"
#include "Text.h"
#include "Grid.h"
#include "GameRunning.h"
#include "GameMainMenu.h"

#include <vector>
#include <iomanip>
#include <time.h>
#include <sstream>  

/* Get Singleton Instances for easy access */
PuyoPuyo* puyoPuyo = puyoPuyo->GetInstance();
GameWindow* gameWindow = gameWindow->GetInstance();
Timer* timer = timer->GetInstance();

int main(int argc, char* argv[])
{
	/* Initializations */
	srand(time(NULL));
	gameWindow->InitializeGameWindow("PuyoPuyoOnABudget", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
	puyoPuyo->InitializeGame(8, 18, new GameMainMenu);


	const int FPS = 60;
	const int frameDelay = 1000 / FPS; // 0.02 sec

	Uint32 frameStart;
	int frameTime;


	int seconds = 0;

	while (puyoPuyo->IsGameRunning())
	{
		/* Capping framerate to 60 FPS */
		Uint64 startTime = SDL_GetTicks64();
		if (startTime >= 1000) startTime = 0;
		frameStart = SDL_GetTicks();
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
		/*                             */


		if (timer->GetDeltaTime() > 0.75f) // Every 0.75 seconds, balls fall down
		{
			if (puyoPuyo->GetBallSetLeaders()[0] != nullptr)
			{
				if (!puyoPuyo->GetBallSetLeaders()[0]->IsMoving())
				{
					puyoPuyo->GetBallSetLeaders()[0]->SetMovingState(true);
					puyoPuyo->GetBallSetLeaders()[0]->GetSecondBall()->SetMovingState(true);
				}
				puyoPuyo->GetBallSetLeaders()[0]->MoveBallSetDown();
			}
			seconds++;
			Timer::GetInstance()->ResetTimer();
		}


		if (puyoPuyo->GetBallSetLeaders()[2] == nullptr && timer->GetTimeScale() > 0) // If new ball has spawn we need to spawn a new one and it will be place on the next balls area
		{
			SDL_Rect destB1 = { 3, 1, 32, 32 };
			SDL_Rect destB2 = { 3, 0, 32, 32 };

			Ball* ball2 = new Ball({ 0, 0, 0, 0 }, destB2, AssetManager::GetInstance()->GetRandomEntityTexture(), Entity::EntityType::Moveable, nullptr);
			Ball* ball1 = new Ball({ 0, 0, 0, 0 }, destB1, AssetManager::GetInstance()->GetRandomEntityTexture(), Entity::EntityType::Moveable, ball2);

			if (puyoPuyo->AddToBallSetLeaders(ball1) > 0)
			{
				ball2->SetMovingState(false);
				ball1->SetMovingState(false);
			}
		}


		puyoPuyo->HandleEvents();
		puyoPuyo->Update();
	}

	return 0;
}