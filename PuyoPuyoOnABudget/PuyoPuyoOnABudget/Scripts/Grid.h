#pragma once


#include "Ball.h"

class Grid
{
	public:
		Grid(int gridWidth, int gridHeigth);

		Ball* GetBallFromGridPos(int xPos, int yPos);
		void SetGridPos(int xPos, int yPos, Ball* ball);
		void ClearGrid();
		void PrintGrid();
		int GetHeight();
		int GetWidth();

	private:
		int width, heigth;
		Ball*** gameGrid;
};

