#pragma once

#include "../../SDL2/include/SDL.h"

#include <iostream>


/* Basic class for every things that gets rendered */
class Entity
{

public:
	enum EntityType { Moveable, Static, Text, MainMenu, MainMenuText, GamePaused, GamePausedText};

	Entity(SDL_Rect srcRec, SDL_Rect destRect, SDL_Texture* texture, EntityType type);
	Entity() {};
	~Entity();


	SDL_Rect GetSRCRect();
	void SetSRCRect(SDL_Rect srcRect);
	SDL_Rect GetDESTRect();
	void SetDESTRect(SDL_Rect destRect);

	SDL_Texture* GetTexture();
	void SetTexture(SDL_Texture* texture);

	bool IsMoving();
	void SetMovingState(bool entMoving);


protected:
	SDL_Texture* entTexture;
	SDL_Rect src, dest;
	bool movingAtTheMoment;
	EntityType entityType;
};