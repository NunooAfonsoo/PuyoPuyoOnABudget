#include "Ball.h"
#include "PuyoPuyo.h"
#include "AssetManager.h"

#include <algorithm>


Ball::Ball(SDL_Rect srcRec, SDL_Rect destRect, std::tuple<SDL_Texture*, Color> textureColor, EntityType type, Ball* secondBall) :
	Entity(srcRec, destRect, std::get<SDL_Texture*>(textureColor), type), secBall(secondBall)
{
	ChangeBallGridPos(destRect.x, destRect.y);
	ballColor = std::get<Color>(textureColor);

	PuyoPuyo::GetInstance()->AddToGameBalls(this);

	if (GetSecondBall() != nullptr) //if this ball is the sets leader
	{
		SetBallSetOrientation(3);
		GetSecondBall()->SetMovingState(true);
	}
	
	dest = CoordinatesToSDL_Rect(dest);

	ballHelperDest = dest; 
	ballHelperDest.y = GetBallFinalYPos() * 33 + 65;
	SetBallHelperTexture(AssetManager::GetInstance()->GetBallHelperTexture(ballColor));
}


Ball::~Ball()
{
	PuyoPuyo::GetInstance()->GameGrid->SetGridPos(gridPos.xPos, gridPos.yPos, nullptr);
	ClearConnectedBalls();
}


void Ball::SetGridPos(int xPosition, int yPosition)
{
	gridPos.xPos = xPosition;
	gridPos.yPos = yPosition;
}


/* Gets ball position in Grid */
Ball::GridPos Ball::GetGridPos()
{
	return gridPos;
}


/* Translates Grid ball's position to pixels on screen for Dest Rect*/
SDL_Rect Ball::CoordinatesToSDL_Rect(SDL_Rect rect)
{
	rect.x = 513 + 33 * gridPos.xPos;  //background start pos x = 512 * y=64 + 1px for each because of edge -> 513, 65
	rect.y = 65 + 33 * gridPos.yPos;   //background start pos x = 512 * y=64 + 1px for each because of edge -> 513, 65
	return rect;
}


/* Translates Dest Rect pixels on screen for to ball's position on Grid */
Ball::GridPos Ball::SDL_RectToCoordinates()
{
	int xCoord = (dest.x - 513) / 32;
	int yCoord = (dest.y - 65) / 32;

	GridPos convertedPos = { xCoord, yCoord };
	return convertedPos;
}

/* Ball's final position if movement always continues down*/
int Ball::GetBallFinalYPos()
{
	for (int y = PuyoPuyo::GetInstance()->GameGrid->GetHeight() - 1; y >= 2; y--)
	{
		if (PuyoPuyo::GetInstance()->GameGrid->GetBallFromGridPos(gridPos.xPos, y) == nullptr || (PuyoPuyo::GetInstance()->GameGrid->GetBallFromGridPos(gridPos.xPos, y) == this)) return y;
	}
	return 0;
}


SDL_Rect Ball::GetHelperDestRect()
{
	return ballHelperDest;
}


void Ball::SetHelperDestRect(SDL_Rect rect)
{
	ballHelperDest = rect;
}

/* Calculates Helper ball's position. When ballSetOrientation == 1/3 one helper ball has to be on top of the other */
void Ball::CalculateBallHelpersPos()
{
	if (GetBallSetOrientation() == 1)
	{
		SDL_Rect leaderRect = GetHelperDestRect();
		leaderRect.y = GetBallFinalYPos() * 33 + 65;
		leaderRect.y -= 33;			// 33 pixels up since 32*32 texture + 1 of grid border
		SetHelperDestRect(leaderRect);
	}
	else if (GetBallSetOrientation() == 3)
	{
		SDL_Rect secondBallRect = GetSecondBall()->GetHelperDestRect();
		secondBallRect.y = GetSecondBall()->GetBallFinalYPos() * 33 + 65;
		secondBallRect.y -= 33;		// 33 pixels up since 32*32 texture + 1 of grid border
		GetSecondBall()->SetHelperDestRect(secondBallRect);
	}
	else
	{
		SDL_Rect leaderRect = GetHelperDestRect();
		SDL_Rect secondBallRect = GetSecondBall()->GetHelperDestRect();
		leaderRect.y = GetBallFinalYPos() * 33 + 65;
		secondBallRect.y = GetSecondBall()->GetBallFinalYPos() * 33 + 65;
		SetHelperDestRect(leaderRect);
		GetSecondBall()->SetHelperDestRect(secondBallRect);
	}

}


int Ball::GetBallSetOrientation()
{
	return ballSetOrientation;
}

void Ball::SetBallSetOrientation(int orientation)
{
	ballSetOrientation = orientation;
}


void Ball::MoveBallSetRight()
{
	switch (ballSetOrientation)
	{
	case 1:
		if (GetSecondBall()->CanMove(0))
		{
			MoveRight();
			GetSecondBall()->MoveRight();
		}
		break;
	default:
		if (CanMove(0))
		{
			GetSecondBall()->MoveRight();
			MoveRight();
		}
		break;
	}
}


void Ball::MoveBallSetLeft()
{
	switch (ballSetOrientation)
	{
	case 1:
		if (GetSecondBall()->CanMove(2))
		{
			MoveLeft();
			GetSecondBall()->MoveLeft();
		}
		break;
	default:
		if (CanMove(2))
		{
			GetSecondBall()->MoveLeft();
			MoveLeft();
		}
		break;
	}
}


void Ball::MoveBallSetDown()
{
	switch (ballSetOrientation)
	{
	case 1:
		if (GetGridPos().yPos + 1 == GetBallFinalYPos() || GetSecondBall()->GetGridPos().yPos + 1 == GetSecondBall()->GetBallFinalYPos())
		{
			SimulateBallSetEntireMovement();  // If next movement is the last one we can't move both balls down since one might deleted. Avoids nullptr Exception
		}
		else 
		{
			GetSecondBall()->MoveDown();
			MoveDown();
		}
		break;
	default:
		if (GetGridPos().yPos + 1 == GetBallFinalYPos() || GetSecondBall()->GetGridPos().yPos + 1 == GetSecondBall()->GetBallFinalYPos())
		{
			SimulateBallSetEntireMovement();  // If next movement is the last one we can't move both balls down since one might deleted. Avoids nullptr Exception
		}
		else
		{

			MoveDown();
			if(PuyoPuyo::GetInstance()->GetBallSetLeaders()[0] != nullptr)
				GetSecondBall()->MoveDown();
		}
		break;
	}
}


void Ball::SimulateBallSetEntireMovement()
{
	if(GetBallSetOrientation() == 1)  // If second ball is below we need to simulate that movement first
	{
		GetSecondBall()->SimulateBallEntireMovement();
		SimulateBallEntireMovement();
	}
	else
	{
		SimulateBallEntireMovement();
		GetSecondBall()->SimulateBallEntireMovement();
	}
	PuyoPuyo::GetInstance()->BallSetReachedGround();
}


void Ball::RotateBallSetRight()
{
	if (CanRotateSecondBallRight(GetBallSetOrientation()))
	{
		if (GetBallSetOrientation() == 0 && GetSecondBall()->GetBallFinalYPos() != GetSecondBall()->GetGridPos().yPos + 1 || GetBallSetOrientation() != 0)
		{
			GetSecondBall()->RotateRight(GetBallSetOrientation());
			if (ballSetOrientation < 3) SetBallSetOrientation(GetBallSetOrientation() + 1);
			else SetBallSetOrientation(0);
		}
	}
}


void Ball::RotateBallSetLeft()
{
	if (CanRotateSecondBallLeft(GetBallSetOrientation()))
	{
		if (GetBallSetOrientation() == 2 && GetSecondBall()->GetBallFinalYPos() != GetSecondBall()->GetGridPos().yPos + 1 || GetBallSetOrientation() != 2)
		{
			GetSecondBall()->RotateLeft(GetBallSetOrientation());
			if (GetBallSetOrientation() > 0) SetBallSetOrientation(GetBallSetOrientation() - 1);
			else SetBallSetOrientation(3);
		}
	}
}


Ball::Color Ball::GetColor()
{
	return ballColor;
}


/* Recursively updates the ball's connections and its connection's connections*/
void Ball::UpdateConnectedBalls(std::vector<Ball*> balls)
{
	balls.push_back(this);

	PuyoPuyo* puyoPuyo = puyoPuyo->GetInstance();
	Ball* ballRight = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos + 1, gridPos.yPos);
	Ball* ballDown = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos, gridPos.yPos + 1);
	Ball* ballLeft = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos - 1, gridPos.yPos);
	Ball* ballUp = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos, gridPos.yPos - 1);

	if (ballRight != nullptr && ballRight->GetColor() == GetColor())
	{
		connectedBalls[0] = ballRight;
		if (!IsMoving()) ballRight->SetTexture(AssetManager::GetInstance()->GetSquareTexture(GetColor()));
		if(!BallInVector(balls, ballRight)) ballRight->UpdateConnectedBalls(balls);
	}
	if (ballDown != nullptr && ballDown->GetColor() == GetColor())
	{
		connectedBalls[1] = ballDown;
		if (!IsMoving()) ballDown->SetTexture(AssetManager::GetInstance()->GetSquareTexture(GetColor()));
		if(!BallInVector(balls, ballDown)) ballDown->UpdateConnectedBalls(balls);
	}	
	if (ballLeft != nullptr && ballLeft->GetColor() == GetColor())
	{
		connectedBalls[2] = ballLeft;
		if (!IsMoving()) ballLeft->SetTexture(AssetManager::GetInstance()->GetSquareTexture(GetColor()));
		if(!BallInVector(balls, ballLeft)) ballLeft->UpdateConnectedBalls(balls);
	}	
	if (ballUp != nullptr && ballUp->GetColor() == GetColor())
	{
		connectedBalls[3] = ballUp;
		if(!IsMoving() ) ballUp->SetTexture(AssetManager::GetInstance()->GetSquareTexture(GetColor()));
		if(!BallInVector(balls, ballUp)) ballUp->UpdateConnectedBalls(balls);
	}

	if (connectedBalls[0] == nullptr && connectedBalls[1] == nullptr && connectedBalls[2] == nullptr && connectedBalls[3] == nullptr)
	{
		SetTexture(AssetManager::GetInstance()->GetBallTextureFromColor(GetColor()));
	}
}

/* Recursively goes through the ball's connections and its connection's connections and returns all the connected balls */
std::vector<Ball*> Ball::GetConnectionMaxScore(std::vector<Ball*> balls)
{
	balls.push_back(this);
	for (int i = 0; i < 4; i++)
	{
		if (connectedBalls[i] != nullptr && !BallInVector(balls, connectedBalls[i]))
		{
			balls = connectedBalls[i]->GetConnectionMaxScore(balls);
		}
	}
	return balls;
}


Ball** Ball::GetConnectedBalls()
{
	return connectedBalls;
}


void Ball::ClearConnectedBalls()
{
	std::fill_n(connectedBalls, 4, nullptr);
}


void Ball::ClearConnectedBall(Ball* ball) 
{
	for (int i = 0; i < 4; i++)
	{
		if (connectedBalls[i] == ball) connectedBalls[i] = nullptr;
	}
}


bool Ball::BallInVector(std::vector<Ball*> vector, Ball* ball)
{
	for (int i = 0; i < vector.size(); i++)
	{
		if (vector[i] == ball) return true;
	}
	return false;
}


Ball* Ball::GetSecondBall()
{
	return secBall;
}

SDL_Texture* Ball::GetBallHelperTexture()
{
	return ballHelperTexture;
}

void Ball::SetBallHelperTexture(SDL_Texture* texture)
{
	ballHelperTexture = texture;
}

/* Updates ball's position given a direction and if after updating position the ball has reached its final destination calculates connections and possible score */
void Ball::UpdatePos(int direction)
{
	if (CanMove(direction))
	{
		switch (direction)
		{
		case 0:
			ChangeBallGridPos(gridPos.xPos + 1, gridPos.yPos);
			break;
		case 1:
			ChangeBallGridPos(gridPos.xPos, gridPos.yPos + 1);
			break;
		case 2:
			ChangeBallGridPos(gridPos.xPos - 1, gridPos.yPos);
			break;
		case 3:
			ChangeBallGridPos(gridPos.xPos, gridPos.yPos - 1);
			break;
		}
	}
	if (!CanMove(1))
	{
		PuyoPuyo::GetInstance()->GameGrid->PrintGrid();

		if (IsMoving())
		{
			PuyoPuyo::GetInstance()->BallSetReachedGround();
		}
		SetMovingState(false);
	}
}


void Ball::MoveRight()
{
	UpdatePos(0);
}


void Ball::MoveLeft()
{
	UpdatePos(2);
}


void Ball::MoveDown()
{
	this;
	UpdatePos(1);
}


/* Simulates the ball's entire movement, aka teleports it to its final position and calculates new possible connections */
void Ball::SimulateBallEntireMovement()
{
	if (IsMoving())
	{
		ChangeBallGridPos(gridPos.xPos, GetBallFinalYPos());
		SetMovingState(false);

		std::vector<Ball*> balls;
		UpdateConnectedBalls(balls);
	}
	else 
	{
		if(CanMove(1))
		{
			SetMovingState(true);
			SimulateBallEntireMovement();
		}
	}
}


/* Receives a direction and returns true if position is free */
bool Ball::CanMove(int direction)
{
	Ball* ball;
	PuyoPuyo* puyoPuyo = puyoPuyo->GetInstance();
	switch (direction)
	{
	case 0:
		if ((GetSecondBall() != nullptr && GetSecondBall()->GetGridPos().yPos < 1) || GetGridPos().yPos < 1) return false;	// Can only move right if the whole set is visible inside the grid
		ball = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos + 1, gridPos.yPos);
		if ((gridPos.xPos < puyoPuyo->GameGrid->GetWidth() - 1 && ball == nullptr) || (ball != nullptr && ball->IsMoving() && ball->CanMove(0))) return true;
		return false;
	case 1:

		ball = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos, gridPos.yPos + 1);
		if ((gridPos.yPos < puyoPuyo->GameGrid->GetHeight() - 1 && ball == nullptr) || (ball != nullptr && ball->IsMoving() && ball->CanMove(1))) return true;
		return false;
	case 2:
		if ((GetSecondBall() != nullptr && GetSecondBall()->GetGridPos().yPos < 1) || GetGridPos().yPos < 1) return false;	// Can only move left if the whole set is visible inside the grid

		ball = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos - 1, gridPos.yPos);
		if ((gridPos.xPos > 0 && ball == nullptr) || (ball != nullptr && ball->IsMoving() && ball->CanMove(2))) return true;
		return false;
	case 3:
		ball = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos, gridPos.yPos - 1);
		if ((gridPos.yPos > 0 && ball == nullptr) || (ball != nullptr && ball->IsMoving() && ball->CanMove(3))) return true;
		return false;
	default:
		std::cout << "Not a Valid Direction: Choose an int from 0 to 3, (0 Right), (1 Down), (2 Left), (3 Up)" << std::endl;
		break;
	}
	return false;
}


void Ball::RotateLeft(int startOrientation)
{
	switch (startOrientation)
	{
	case 0:
		UpdatePos(3);
		UpdatePos(2);
		break;
	case 1:
		UpdatePos(0);
		UpdatePos(3);
		break;
	case 2:
		UpdatePos(1);
		UpdatePos(0);
		break;
	case 3:
		UpdatePos(2);
		UpdatePos(1);
		break;
	default:
		std::cout << "Not a Valid StartOrientation: Choose an int from 0 to 3, (0 right), (1 down), (2 left), (3 up)" << std::endl;
		break;
	}
}


void Ball::RotateRight(int startOrientation)
{
	switch (startOrientation)
	{
	case 0:
		UpdatePos(1);
		UpdatePos(2);
		break;
	case 1:
		UpdatePos(2);
		UpdatePos(3);
		break;
	case 2:
		UpdatePos(3);
		UpdatePos(0);
		break;
	case 3:
		UpdatePos(0);
		UpdatePos(1);
		break;
	default:
		std::cout << "Not a Valid StartOrientation: Choose an int from 0 to 3, (0 right), (1 down), (2 left), (3 up)" << std::endl;
		break;
	}
}


bool Ball::CanRotateSecondBallRight(int startOrientation)
{
	Ball* ball1; 
	Ball* ball2;
	PuyoPuyo* puyoPuyo = puyoPuyo->GetInstance();

	switch (startOrientation)
	{
	case 0:
		ball1 = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos, gridPos.yPos + 1);

		if ((gridPos.yPos < puyoPuyo->GameGrid->GetHeight() - 1 && ball1 == nullptr) || (ball1 != nullptr && ball1->IsMoving())) return true;
		return false;
	case 1:
		ball1 = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos - 1, gridPos.yPos);
		ball2 = puyoPuyo->GameGrid->GetBallFromGridPos(GetSecondBall()->GetGridPos().xPos - 1, GetSecondBall()->GetGridPos().yPos + 1);
		if ((ball2 == nullptr) && (gridPos.xPos > 0 && ball1 == nullptr) || (ball1 != nullptr && ball1->IsMoving())) return true;
		return false;
	case 2:
		ball1 = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos, gridPos.yPos - 1);

		if ((gridPos.yPos > 1 && ball1 == nullptr) || (ball1 != nullptr && ball1->IsMoving())) return true;
		return false;
	case 3:
		if ((gridPos.yPos < 2)) return false;
		ball1 = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos + 1, gridPos.yPos);

		if ((gridPos.xPos < puyoPuyo->GameGrid->GetWidth() - 1 && ball1 == nullptr) || (ball1 != nullptr && ball1->IsMoving())) return true;
		return false;
	default:
		std::cout << "Not a Valid Direction: Choose an int from 0 to 3, (0 right), (1 down), (2 left), (3 up)" << std::endl;
		break;
	}
	return false;

}

bool Ball::CanRotateSecondBallLeft(int startOrientation)
{
	Ball* ball;
	Ball* ball2;
	PuyoPuyo* puyoPuyo = puyoPuyo->GetInstance();

	switch (startOrientation)
	{
	case 0:
		ball = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos - 1, gridPos.yPos - 1);

		if ((gridPos.yPos > 1 && ball == nullptr) || (ball != nullptr && ball->IsMoving())) return true;
		return false;
	case 1:
		ball = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos + 1, gridPos.yPos);
		ball2 = puyoPuyo->GameGrid->GetBallFromGridPos(GetSecondBall()->GetGridPos().xPos + 1, GetSecondBall()->GetGridPos().yPos + 1);

		if ((ball2 == nullptr) && (gridPos.xPos < puyoPuyo->GameGrid->GetWidth() - 1 && ball == nullptr) || (ball != nullptr && ball->IsMoving())) return true;
		return false;
	case 2:
		ball = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos, gridPos.yPos + 1);

		if ((gridPos.yPos < puyoPuyo->GameGrid->GetHeight() - 1 && ball == nullptr) || (ball != nullptr && ball->IsMoving())) return true;
		return false;
	case 3:
		if ((gridPos.yPos < 2)) return false;
		ball = puyoPuyo->GameGrid->GetBallFromGridPos(gridPos.xPos - 1, gridPos.yPos);

		if ((gridPos.xPos > 0 && ball == nullptr) || (ball != nullptr && ball->IsMoving())) return true;
		return false;
	default:
		std::cout << "Not a Valid Direction: Choose an int from 0 to 3, (0 right), (1 down), (2 left), (3 up)" << std::endl;
		break;
	}
	return false;
}


/* Changes ball position in the Grid */
void Ball::ChangeBallGridPos(int xPos, int yPos)
{
	PuyoPuyo* puyoPuyo = puyoPuyo->GetInstance();

	puyoPuyo->GameGrid->SetGridPos(gridPos.xPos, gridPos.yPos, nullptr); // Setting the old ball's position to null
	SetGridPos(xPos, yPos);
	puyoPuyo->GameGrid->SetGridPos(gridPos.xPos, gridPos.yPos, this);    // Setting the new ball's position to ball

	/* Update helper ball's position */
	dest = CoordinatesToSDL_Rect(dest);
	ballHelperDest = dest;
	ballHelperDest.y = GetBallFinalYPos() * 33 + 65;
	dest = CoordinatesToSDL_Rect(ballHelperDest);
}