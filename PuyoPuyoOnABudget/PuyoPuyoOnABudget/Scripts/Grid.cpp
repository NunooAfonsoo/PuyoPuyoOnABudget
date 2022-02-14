#include "Grid.h"

#include "Ball.h"

Grid::Grid(int gridWidth, int gridHeigth) : 
	width(gridWidth), heigth(gridHeigth)
{

	gameGrid = new Ball** [gridWidth];

	for (int x = 0; x < gridWidth; x++)
		gameGrid[x] = new Ball * [gridHeigth];

	for (int x = 0; x < gridWidth; x++) {
		for (int y = 0; y < gridHeigth; y++) {
			SetGridPos(x, y, nullptr);
		}
	}
}

Ball* Grid::GetBallFromGridPos(int xPos, int yPos)
{
	if (xPos >= width || yPos >= heigth || xPos < 0 || yPos < 2) return nullptr;
	return gameGrid[xPos][yPos];
}

void Grid::SetGridPos(int xPos, int yPos, Ball* ball)
{
	gameGrid[xPos][yPos] = ball;
}

void Grid::ClearGrid()
{
	for (int y = heigth - 1; y >= 1; y--)
	{
		for (int x = width - 1; x >= 0; x--)
		{
			SetGridPos(x, y, nullptr);
		}
	}
}

void Grid::PrintGrid()
{
	for (int y = 2; y < heigth; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (GetBallFromGridPos(x, y) == nullptr)
				std::cout << "0 ";
			else
			{
				Ball* ball = GetBallFromGridPos(x, y);
				if (ball->GetColor() == Ball::Color::Red) std::cout << "1 ";
				else if (ball->GetColor() == Ball::Color::Green) std::cout << "2 ";
				else if (ball->GetColor() == Ball::Color::Blue) std::cout << "3 ";
				else if (ball->GetColor() == Ball::Color::Purple) std::cout << "4 ";

			}
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}

int Grid::GetHeight()
{
	return heigth;
}

int Grid::GetWidth()
{
	return width;
}





