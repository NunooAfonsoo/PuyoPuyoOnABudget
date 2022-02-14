#include "PuyoPuyo.h"
#include "AssetManager.h"
#include "GameMainMenu.h"

#include <math.h>
#include <sstream>  
#include <iostream>  


PuyoPuyo* PuyoPuyo::instance = NULL;


#pragma region Initialization/Restart

PuyoPuyo::PuyoPuyo()
{
}

PuyoPuyo::~PuyoPuyo()
{
	timer->ReleaseInstance();
	delete gameState;
	gameWindow->ReleaseInstance();
}

PuyoPuyo* PuyoPuyo::GetInstance()
{
	if (!instance)
		instance = new PuyoPuyo();
	return instance;
}

void PuyoPuyo::ReleaseInstance()
{
	delete this;
}


/* Initializes the game grid */
void PuyoPuyo::InitializeGame(int xSize, int ySize, GameState* state)
{
	SetGameState(state);

	gameWindow = gameWindow->GetInstance();
	timer = timer->GetInstance();
	timer->SetTimeScale(0);

	GameGrid = new Grid(xSize, ySize);
}



void PuyoPuyo::RestartGame()
{
	GameGrid->ClearGrid();
	ClearBallsConnections();
	ClearBallSetLeaders();
	gameBalls.clear();
	textEntites.clear();
	unmoveableEntities.clear();
	mainMenuTextEntites.clear();
	mainMenuEntites.clear();
	gamePausedTextEntites.clear();
	gamePausedEntites.clear();

}


void PuyoPuyo::ClearBallsConnections()
{
	for (Ball* ball : gameBalls)
	{
		ball->ClearConnectedBalls();
	}
}


void PuyoPuyo::ClearBallSetLeaders()
{
	for (int i = 0; i < 3; i++)
	{
		ballSetLeaders[i] = nullptr;
	}
}


bool PuyoPuyo::IsGameRunning()
{
	return gameIsRunning;
}

void PuyoPuyo::SetGameState(GameState* state)
{
	if (gameState != nullptr)
		delete gameState;

	gameState = state;
	gameState->SetGame(this);
}


void PuyoPuyo::CheckForGameLoss()
{
	Ball* ball = GameGrid->GetBallFromGridPos(3, 2);
	if (GetBallSetLeaders()[0] != nullptr && GetBallSetLeaders()[0]->GetBallFinalYPos() <= 2) // If can't place new balls, reset game after 3 seconds
	{
		timer->SetTimeScale(0);
		timer->SetGameSpeed(0);

		gameOverText->UpdateText("Game Over! Final Score: " + std::to_string(GetGameScore()));

		gameState->Update();
		RestartGame();

		SDL_Delay(3000);

		timer->SetGameSpeed(1);
		InitializeGame(8, 18, new GameMainMenu);
	}
}

void PuyoPuyo::SetGameIsRunning(bool gameIsRunning)
{
	this->gameIsRunning = gameIsRunning;
}

#pragma endregion 



#pragma region Rendering/EventHangling


void PuyoPuyo::Update()
{
	if (gameState != nullptr) gameState->Update();
	CheckForGameLoss();
}


void PuyoPuyo::HandleEvents()
{
	if(gameState != nullptr) gameState->HandleEvents();
}

void PuyoPuyo::RenderMainMenuEntities()
{
	for (Entity* ent : GetMainMenuEntities())
		gameWindow->Render(ent->GetTexture(), ent->GetDESTRect());

	for (Text* text : GetMainMenuTextEntities())
	{
		gameWindow->RenderText(text, text->GetDESTRect());
	}
}

void PuyoPuyo::RenderGamePausedEntities()
{
	for (Entity* ent : GetGamePausedEntities())
		gameWindow->Render(ent->GetTexture(), ent->GetDESTRect());

	for (Text* text : GetGamePausedTextEntities())
	{
		gameWindow->RenderText(text, text->GetDESTRect());
	}
}


void PuyoPuyo::RenderAllEntities()
{
	for (Entity* ent : GetUnmoveableEntities())
		gameWindow->Render(ent->GetTexture(), ent->GetDESTRect());

	for (Text* text : GetTextEntities())
	{
		gameWindow->RenderText(text, text->GetDESTRect());
	}

	std::vector<Ball*> nextBalls;
	for (Ball* ball : GetBalls())
	{
		if (GetBallSetLeaders()[1] != nullptr && (ball == GetBallSetLeaders()[1] || ball == GetBallSetLeaders()[1]->GetSecondBall()))
		{
			Ball* leader = GetBallSetLeaders()[1];
			Ball* secondBall = leader->GetSecondBall();

			leader->SetDESTRect({ 800, 197, 32, 32 });
			secondBall->SetDESTRect({ 800, 164, 32, 32 });
			gameWindow->Render(leader->GetTexture(), leader->GetDESTRect());
			gameWindow->Render(secondBall->GetTexture(), secondBall->GetDESTRect());
		}
		else if (GetBallSetLeaders()[2] != nullptr && (ball == GetBallSetLeaders()[2] || ball == GetBallSetLeaders()[2]->GetSecondBall()))
		{
			Ball* leader = GetBallSetLeaders()[2];
			Ball* secondBall = leader->GetSecondBall();

			leader->SetDESTRect({ 833, 230, 32, 32 });
			secondBall->SetDESTRect({ 833, 197, 32, 32 });
			gameWindow->Render(leader->GetTexture(), leader->GetDESTRect());
			gameWindow->Render(secondBall->GetTexture(), secondBall->GetDESTRect());
		}
		else if (ball->GetGridPos().yPos > 1)
		{
			gameWindow->Render(ball->GetTexture(), ball->GetDESTRect());
			if (ball->IsMoving() && ball->GetSecondBall() != nullptr)
			{
				ball->CalculateBallHelpersPos();
				gameWindow->Render(ball->GetBallHelperTexture(), ball->GetHelperDestRect());
				gameWindow->Render(ball->GetSecondBall()->GetBallHelperTexture(), ball->GetSecondBall()->GetHelperDestRect());
			}
		}
	}
}

#pragma endregion


#pragma region VectorsOperations


void PuyoPuyo::AddToGameBalls(Ball* ball)
{
	gameBalls.push_back(ball);
}


std::vector<Ball*> PuyoPuyo::GetBalls()
{
	return gameBalls;
}


void PuyoPuyo::AddToUnmoveableEntities(Entity* entity)
{
	unmoveableEntities.push_back(entity);
}


std::vector<Entity*> PuyoPuyo::GetUnmoveableEntities()
{
	return unmoveableEntities;
}


void PuyoPuyo::EraseBallFromGameBalls(int ballIndex)
{
	Ball* ball = gameBalls[ballIndex];

	Ball::GridPos ballPos = ball->GetGridPos();
	GameGrid->SetGridPos(ballPos.xPos, ballPos.yPos, nullptr);
	delete gameBalls[ballIndex];
	gameBalls.erase(gameBalls.begin() + ballIndex);
}


void PuyoPuyo::EraseTextFromTextEntities(int index)
{
	
	Text* ball = textEntites[index];

	textEntites.erase(textEntites.begin() + index);
}


void PuyoPuyo::AddToTextEntities(Text* text)
{
	textEntites.push_back(text);
}


std::vector<Text*> PuyoPuyo::GetTextEntities()
{
	return textEntites;
}

void PuyoPuyo::AddToMainMenuEntities(Entity* entity)
{
	mainMenuEntites.push_back(entity);
}

void PuyoPuyo::AddToMainMenuTextEntities(Text* text)
{
	mainMenuTextEntites.push_back(text);
}

std::vector<Entity*> PuyoPuyo::GetMainMenuEntities()
{
	return mainMenuEntites;
}

std::vector<Text*> PuyoPuyo::GetMainMenuTextEntities()
{
	return mainMenuTextEntites;
}

void PuyoPuyo::AddToGamePausedEntities(Entity* entity)
{
	gamePausedEntites.push_back(entity);
}

void PuyoPuyo::AddToGamePausedTextEntities(Text* text)
{
	gamePausedTextEntites.push_back(text);
}

std::vector<Entity*> PuyoPuyo::GetGamePausedEntities()
{
	return gamePausedEntites;
}

std::vector<Text*> PuyoPuyo::GetGamePausedTextEntities()
{
	return gamePausedTextEntites;
}

void PuyoPuyo::ClearGamePausedEntities()
{
	gamePausedEntites.clear();
	gamePausedTextEntites.clear();
}

void PuyoPuyo::ClearScoreTextEntity()
{
	scoreText->UpdateText("");
}


#pragma endregion


#pragma region BallsReachingGround

void PuyoPuyo::BallSetReachedGround()
{
	if (ballSetLeaders[0] != nullptr) 
	{
		Ball::GridPos gridPos = ballSetLeaders[0]->GetGridPos();
		std::set<Ball*> balls;

		Ball::Color leaderColor = ballSetLeaders[0]->GetColor();
		Ball::Color secondBallColor = ballSetLeaders[0]->GetSecondBall()->GetColor();
		Ball* leader = ballSetLeaders[0];
		Ball* secondBall= ballSetLeaders[0]->GetSecondBall();

		

		/* if ball set is in a ledge simulate the other ball's movement */
		if (leader->CanMove(1))
		{
			leader->SimulateBallEntireMovement();
		}
		if (secondBall->CanMove(1))
		{
			secondBall->SimulateBallEntireMovement();
		}
		
		leader->SetMovingState(false);
		secondBall->SetMovingState(false);

		std::vector<Ball*> leaderConnections, secondBallConnections;

		leaderConnections = UpdateBall(leader);
		secondBallConnections = UpdateBall(secondBall);

		int scoreLeader = 0, scoreSecond = 0;
		scoreLeader = leaderConnections.size();
		scoreSecond = secondBallConnections.size();

		if (scoreLeader >= 4)
		{
			DeleteSetOfBalls(leaderConnections);
			AddToGameScore(scoreLeader);
		}
		else if (scoreSecond >= 4 && leaderColor != secondBallColor)
		{
			DeleteSetOfBalls(secondBallConnections);
			AddToGameScore(scoreSecond);
		}

		/* Need to clear balls connections, and check again for connections before 
		calculating new scores since some balls migth fall and lose/create new connections */
		ClearBallsConnections();

		std::vector<Ball*> ballsVec;
		for (int y = GameGrid->GetHeight() - 1; y >= 0; y--)
		{
			for (int x = GameGrid->GetWidth() - 1; x >= 0; x--)
			{
				if (GameGrid->GetBallFromGridPos(x, y) != nullptr)
				{
					Ball* ball = GameGrid->GetBallFromGridPos(x, y);
					ball->UpdateConnectedBalls(ballsVec);
					std::vector<Ball*> balls = UpdateBall(ball);
					int ballScore = balls.size();
					if (ballScore >= 4)
					{
						DeleteSetOfBalls(balls);
						AddToGameScore(ballScore);
					}
				}
			}
		}
	}
	UpdateBallSetLeaders();
}


Ball** PuyoPuyo::GetBallSetLeaders()
{
	return ballSetLeaders;
}


int PuyoPuyo::AddToBallSetLeaders(Ball* ball)
{
	for (int i = 0; i < 3; i++) 
	{
		if (ballSetLeaders[i] == nullptr)
		{
			ballSetLeaders[i] = ball;
			return i;
		}
	}
}


bool PuyoPuyo::BallInBallSetLeaders(Ball* ball)
{
	for (int i = 0; i < 3; i++)
	{
		if (GetBallSetLeaders()[i] == ball) return true;
	}
	return false;
}


void PuyoPuyo::UpdateBallSetLeaders()
{
	ballSetLeaders[0] = ballSetLeaders[1];
	ballSetLeaders[1] = ballSetLeaders[2];
	ballSetLeaders[2] = nullptr;
}


int PuyoPuyo::GetGameScore()
{
	return gameScore;
}


void PuyoPuyo::AddToGameScore(int increment)
{
	int inc = 3 + exp2(increment - 4);
	gameScore += inc;
	gameTimeIncreaser += inc;
	if (gameTimeIncreaser > 15 && Timer::GetInstance()->GetGameSpeed() < 11.5f)
	{
		Timer::GetInstance()->SetTimeScale(Timer::GetInstance()->GetTimeScale() * 1.25f);
		gameTimeIncreaser = 0;
	}
	scoreText->UpdateText("Score: " + std::to_string(gameScore));
}

/*  Updates the ball's connections and then returns the score of those connections aka the amount of connected balls */
std::vector<Ball*> PuyoPuyo::UpdateBall(Ball* ball)
{
	std::vector<Ball*> balls;
	ball->UpdateConnectedBalls(balls);
	balls.clear();
	return ball->GetConnectionMaxScore(balls);
}


/* Re-renders game window with the balls inside 'balls' vector 
   being renderer with an empty texture and then with their normal 
   texture 4 times (2 blinks) */
void PuyoPuyo::BallsClearedBlinkingAnimation(std::vector<Ball*> balls)
{
	gameWindow->ClearGameWindow();

	RenderAllEntities();
	gameWindow->Display();
	SDL_Delay(100);

	for (int i = 0; i < 4; i++)
	{
		if (i % 2 == 0)
		{
			for (Ball* ball : balls)
			{
				ball->SetTexture(AssetManager::GetInstance()->GetEmptyCell());
				gameWindow->Render(ball->GetTexture(), ball->GetDESTRect());
			}
		}
		else 
		{
			for (Ball* ball : balls)
			{
				ball->SetTexture(AssetManager::GetInstance()->GetSquareTexture(ball->GetColor()));
				gameWindow->Render(ball->GetTexture(), ball->GetDESTRect());
			}
		}
		gameWindow->Display();
		SDL_Delay(100);
	}
}


void PuyoPuyo::DeleteSetOfBalls(std::vector<Ball*> balls)
{
	BallsClearedBlinkingAnimation(balls);

	/* Delete connected balls and remove them from the game entities vector */
	for (int i = 0; i < balls.size(); i++)
	{
		for (int j = 0; j < GetBalls().size(); j++)
		{
			if (GetBalls()[j] == balls[i])
			{
				EraseBallFromGameBalls(j);
				break;
			}
		}
	}

	/* Simulate the rest of the balls movement starting from the bottom right, 
	   there might be some balls that need to fall */
	for (int y = GameGrid->GetHeight() - 1; y >= 1; y--)
	{
		for (int x = GameGrid->GetWidth() - 1; x >= 0; x--)
		{
			Ball* ball = GameGrid->GetBallFromGridPos(x, y);
			if (ball != nullptr)
			{
				ball->SimulateBallEntireMovement();
			}
		}
	}
}
#pragma endregion


void PuyoPuyo::SetScoreText(Text* text)
{
	scoreText = text;
}


void PuyoPuyo::SetGameOverText(Text* text)
{
	gameOverText = text;
}

void PuyoPuyo::SetPauseGameText(Text* text)
{
	pauseGameText = text;
}

Text* PuyoPuyo::GetScoreText()
{
	return scoreText;
}


Text* PuyoPuyo::GetGameOverText()
{
	return gameOverText;
}

Text* PuyoPuyo::GetPauseGameText()
{
	return pauseGameText;
}
