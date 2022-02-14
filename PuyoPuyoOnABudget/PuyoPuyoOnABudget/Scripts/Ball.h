#pragma once

#include "../../SDL2/include/SDL.h"
#include "Entity.h"

#include <iostream>
#include <set>
#include <vector>


class Ball : public Entity
{
	public:
		enum Color { Red, Green, Blue, Purple};
		 
		struct GridPos
		{
			int xPos, yPos;
		};

		Ball(SDL_Rect srcRec, SDL_Rect destRect, std::tuple<SDL_Texture*, Color> textureColor, EntityType type, Ball* secondBall);
		~Ball(); 

		void SetGridPos(int xPosition, int yPosition);
		void ChangeBallGridPos(int xPos, int yPos);
		GridPos GetGridPos();
		Color GetColor();
		SDL_Texture* GetBallHelperTexture();
		void SetBallHelperTexture(SDL_Texture* texture);
		int GetBallFinalYPos();
		SDL_Rect GetHelperDestRect();
		void SetHelperDestRect(SDL_Rect rect);
		void CalculateBallHelpersPos();


		SDL_Rect CoordinatesToSDL_Rect(SDL_Rect rect);
		GridPos SDL_RectToCoordinates();


		int GetBallSetOrientation();
		void SetBallSetOrientation(int orientation);
		Ball* GetSecondBall();
		void MoveBallSetRight();
		void MoveBallSetLeft();
		void MoveBallSetDown();
		void SimulateBallSetEntireMovement();
		void RotateBallSetRight();
		void RotateBallSetLeft();
		bool CanRotateSecondBallRight(int startOrientation);
		bool CanRotateSecondBallLeft(int startOrientation);


		void MoveRight();
		void MoveLeft();
		void MoveDown();
		void SimulateBallEntireMovement();
		void RotateRight(int startOrientation); //startOrientation -> 0 right, 1 down, 2 left, 3 up // direction -> 0 right,,, 1 left
		void RotateLeft(int startOrientation); //startOrientation -> 0 right, 1 down, 2 left, 3 up // direction -> 0 right,,, 1 left


		bool CanMove(int direction);
		
		
		void UpdatePos(int direction);


		bool BallInVector(std::vector<Ball*> set, Ball* ball);
		void UpdateConnectedBalls(std::vector<Ball*> balls);
		std::vector<Ball*> GetConnectionMaxScore(std::vector<Ball*> balls);
		Ball** GetConnectedBalls();
		void ClearConnectedBalls();
		void ClearConnectedBall(Ball* ball);


	private:
		GridPos gridPos;
		Ball* secBall;
		int ballSetOrientation;		// 0 right, 1 down, 2 left, 3 up
		Ball* connectedBalls[4] = { nullptr, nullptr, nullptr, nullptr };
		Color ballColor;
		SDL_Texture* ballHelperTexture;
		SDL_Rect ballHelperDest;
};
