#pragma once

#include "PuyoPuyo.h"

class GameMainMenu : public GameState
{
	public:
		GameMainMenu();

		void Update() override;

		void HandleEvents() override;

		void MoveArrow(int direction);

	private:
		Entity* arrow;
		int arrowPos; //0 play game, 1 exit game
};

