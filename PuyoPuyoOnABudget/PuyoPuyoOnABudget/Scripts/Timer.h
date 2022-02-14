#pragma once

#include "SDL.h"


class Timer
{
public:
	static Timer* GetInstance();
	void ReleaseInstance();

	void ResetTimer();
	float GetDeltaTime();
	float GetTimeScale();
	void SetTimeScale(float tScale);
	void Update();
	float GetGameSpeed();
	void SetGameSpeed(float speed);

private:
	Timer();
	static Timer* instance;

	unsigned int startTicks;
	unsigned int elapsedTicks;
	float deltaTime;
	float timeScale;
	float gameSpeed;
};