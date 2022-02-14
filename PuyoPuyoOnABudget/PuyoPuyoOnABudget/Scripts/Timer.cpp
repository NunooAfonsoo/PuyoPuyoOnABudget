#include "Timer.h"

#include <iostream>

Timer* Timer::instance = NULL;


Timer::Timer()
{
	ResetTimer();
	timeScale = 1.0f;
	gameSpeed = 1.0f;
}


Timer* Timer::GetInstance()
{
	if (!instance)
		instance = new Timer();
	return instance;
}


void Timer::ReleaseInstance()
{
	delete this;
}


void Timer::ResetTimer()
{
	startTicks = SDL_GetTicks();
	elapsedTicks = 0;
	deltaTime = 0.0;
}


float Timer::GetDeltaTime()
{
	return deltaTime;
}


float Timer::GetTimeScale()
{
	return timeScale;
}


void Timer::SetTimeScale(float tScale)
{
	timeScale = tScale;
	if (tScale > 0) SetGameSpeed(tScale);
}

/* Updates elapsed time since last frame */
void Timer::Update()
{
	elapsedTicks = SDL_GetTicks() - startTicks;
	deltaTime = elapsedTicks * 0.001f * timeScale;  // conversion to sec
}

float Timer::GetGameSpeed()
{
	return gameSpeed;
}

void Timer::SetGameSpeed(float speed)
{
	gameSpeed = speed;
}
