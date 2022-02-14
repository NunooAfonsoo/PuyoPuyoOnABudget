#pragma once

#include "PuyoPuyo.h"


class GamePaused : public GameState
{
public:
	GamePaused();

	void Update() override;

	void HandleEvents() override;

	void MoveArrow(int direction);

private:
	Entity* arrow;
	int arrowPos; //0 play game, 1 exit game
};

