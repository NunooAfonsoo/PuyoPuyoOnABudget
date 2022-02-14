#pragma once

#include "GameWindow.h"
#include "Entity.h"
#include "Ball.h"
#include "Timer.h"
#include "Grid.h"


#include <vector>


class GameState;


class PuyoPuyo
{
	public:
		static PuyoPuyo* GetInstance();
		void ReleaseInstance();
		void InitializeGame(int xSize, int ySize, GameState* state);		//Initialization of grid and background entities
		void RestartGame();
		void ClearBallSetLeaders();
		void ClearBallsConnections();
		bool IsGameRunning();	
		void SetGameState(GameState* state);
		void CheckForGameLoss();
		void SetGameIsRunning(bool gameIsRunning);


		void HandleEvents();
		void Update();
		void RenderAllEntities();					//Renders all game entities
		void RenderMainMenuEntities();
		void RenderGamePausedEntities();


		/* Game Entities vectors */
		void AddToGameBalls(Ball* ball);
		std::vector<Ball*> GetBalls();
		void AddToUnmoveableEntities(Entity* entity);
		std::vector<Entity*> GetUnmoveableEntities();
		void AddToTextEntities(Text* text);
		std::vector<Text*> GetTextEntities();

		void AddToMainMenuEntities(Entity* entity);
		void AddToMainMenuTextEntities(Text* text);

		std::vector<Entity*> GetMainMenuEntities();
		std::vector<Text*> GetMainMenuTextEntities();

		void AddToGamePausedEntities(Entity* entity);
		void AddToGamePausedTextEntities(Text* text);

		std::vector<Entity*> GetGamePausedEntities();
		std::vector<Text*> GetGamePausedTextEntities();
		void ClearGamePausedEntities();
		void ClearScoreTextEntity();

		void EraseBallFromGameBalls(int ballIndex);
		void EraseTextFromTextEntities(int index);
		/*						 */


		void BallSetReachedGround();
		Ball** GetBallSetLeaders();
		int AddToBallSetLeaders(Ball* ball);
		bool BallInBallSetLeaders(Ball* ball);
		void UpdateBallSetLeaders();
		std::vector<Ball*> UpdateBall(Ball* ball);
		void BallsClearedBlinkingAnimation(std::vector<Ball*> balls);
		void DeleteSetOfBalls(std::vector<Ball*> balls);
		void AddToGameScore(int increment);
		int GetGameScore();
		void SetGameScore(int score);


		void SetScoreText(Text* text);
		void SetGameOverText(Text* text);
		void SetPauseGameText(Text* text);
		Text* GetScoreText();
		Text* GetGameOverText();
		Text* GetPauseGameText();


		Grid* GameGrid;

	private:
		PuyoPuyo();
		~PuyoPuyo();
		static PuyoPuyo* instance;
		bool gameIsRunning = true;

		GameWindow* gameWindow;
		Timer* timer;
		GameState* gameState = nullptr;

		Ball* ballSetLeaders[3] = { nullptr, nullptr, nullptr };   //3 ball set leaders(6 balls in total), 1 leader active and two others for when first one reaches the ground


		Text* scoreText;
		Text* gameOverText;
		Text* pauseGameText;


		/* Game Entities vectors */
		std::vector<Ball*> gameBalls;
		std::vector<Entity*> unmoveableEntities;
		std::vector<Text*> textEntites;

		std::vector<Entity*> mainMenuEntites;
		std::vector<Text*> mainMenuTextEntites;

		std::vector<Entity*> gamePausedEntites;
		std::vector<Text*> gamePausedTextEntites;
		/*						 */


		int gameScore = 0;
		int gameTimeIncreaser = 0;   // variable to increase game speed, Every 15 score game speed increases
};


class GameState
{
public:
	virtual ~GameState() {
	}

	void SetGame(PuyoPuyo* game) {
		this->game = game;
	}

	virtual void Update() {};

	virtual void HandleEvents() {};

protected:
	PuyoPuyo* game = game->GetInstance();
};