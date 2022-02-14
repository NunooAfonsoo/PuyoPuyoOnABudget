#pragma once

#include "PuyoPuyo.h"

class GameRunning : public GameState
{
	public:
		GameRunning(int score);

		void Update() override;

		void HandleEvents() override;
};

