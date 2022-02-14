#include "Entity.h"
#include "AssetManager.h"
#include "PuyoPuyo.h"

Entity::Entity(SDL_Rect srcRec, SDL_Rect destRect, SDL_Texture* texture, EntityType type) :
	src(srcRec), dest(destRect), entTexture(texture), entityType(type)
{
	switch (type)
	{
	case Entity::Moveable:
		SetMovingState(true);
		break;
	case Entity::Static:
		PuyoPuyo::GetInstance()->AddToUnmoveableEntities(this);
		break;
	case Entity::MainMenu:
		PuyoPuyo::GetInstance()->AddToMainMenuEntities(this);
		break;
	case Entity::GamePaused:
		PuyoPuyo::GetInstance()->AddToGamePausedEntities(this);
		break;
	default:
		break;
	}
}

Entity::~Entity()
{
}


SDL_Rect Entity::GetSRCRect()
{
	return src;
}


void Entity::SetSRCRect(SDL_Rect srcRect)
{
	src = srcRect;
}


SDL_Rect Entity::GetDESTRect()
{
	return dest;
}


void Entity::SetDESTRect(SDL_Rect destRect)
{
	dest = destRect;
}


SDL_Texture* Entity::GetTexture()
{
	return entTexture;
}


void Entity::SetTexture(SDL_Texture* texture)
{
	entTexture = texture;
}


bool Entity::IsMoving()
{
	return movingAtTheMoment;
}


void Entity::SetMovingState(bool entMoving)
{
	movingAtTheMoment = entMoving;
}