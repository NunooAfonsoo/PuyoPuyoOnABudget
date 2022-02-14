#include "Text.h"
#include "PuyoPuyo.h"


Text::Text(SDL_Rect srcRec, SDL_Rect destRect, SDL_Texture* entTexture, EntityType type) :
	Entity(srcRec, destRect, entTexture, type)
{
	switch (type)
	{

	case Entity::Text:
		PuyoPuyo::GetInstance()->AddToTextEntities(this);
		break;
	case Entity::MainMenuText:
		PuyoPuyo::GetInstance()->AddToMainMenuTextEntities(this);
		break;
	case Entity::GamePausedText:
		PuyoPuyo::GetInstance()->AddToGamePausedTextEntities(this);
		break;
	default:
		break;	
	}

}


void Text::UpdateText(std:: string updatedText)
{
	text = updatedText;
}


std::string Text::GetText()
{
	return text;
}


void Text::SetColor(SDL_Color newColor)
{
	color = newColor;
}


SDL_Color Text::GetColor()
{
	return color;
}
