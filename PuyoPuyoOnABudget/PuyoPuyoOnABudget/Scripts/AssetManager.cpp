#include "AssetManager.h"

#include "PuyoPuyo.h"
#include "AssetManager.h"


AssetManager* AssetManager::instance = NULL;


AssetManager::AssetManager()
{
	SDL_Texture* RedBall = GetTexture("Assets/RedBall.png");
	balls.emplace(Ball::Color::Red, RedBall);

	SDL_Texture* GreenBall = GetTexture("Assets/GreenBall.png");
	balls.emplace(Ball::Color::Green, GreenBall);

	SDL_Texture* BlueBall = GetTexture("Assets/BlueBall.png");
	balls.emplace(Ball::Color::Blue, BlueBall);

	SDL_Texture* PurpleBall = GetTexture("Assets/PurpleBall.png");
	balls.emplace(Ball::Color::Purple, PurpleBall);


	SDL_Texture* RedSquare = GetTexture("Assets/RedSquare.png");
	squares.emplace(Ball::Color::Red, RedSquare);

	SDL_Texture* GreenSquare = GetTexture("Assets/GreenSquare.png");
	squares.emplace(Ball::Color::Green, GreenSquare);

	SDL_Texture* BlueSquare = GetTexture("Assets/BlueSquare.png");
	squares.emplace(Ball::Color::Blue, BlueSquare);

	SDL_Texture* PurpleSquare = GetTexture("Assets/PurpleSquare.png");
	squares.emplace(Ball::Color::Purple, PurpleSquare);


	SDL_Texture* RedBallHelper = GetTexture("Assets/RedBallHelper.png");
	ballHelpers.emplace(Ball::Color::Red, RedBallHelper);

	SDL_Texture* GreenBallHelper = GetTexture("Assets/GreenBallHelper.png");
	ballHelpers.emplace(Ball::Color::Green, GreenBallHelper);

	SDL_Texture* BlueBallHelper = GetTexture("Assets/BlueBallHelper.png");
	ballHelpers.emplace(Ball::Color::Blue, BlueBallHelper);

	SDL_Texture* PurpleBallHelper = GetTexture("Assets/PurpleBallHelper.png");
	ballHelpers.emplace(Ball::Color::Purple, PurpleBallHelper);

	emptyCell = GetTexture("Assets/Empty.png");

	gridBackground = GetTexture("Assets/Background.png");
	ballsBackground = GetTexture("Assets/BallsBackground.png");
	mainMenuArrow = GetTexture("Assets/Arrow.png");
	gameLogo = GetTexture("Assets/GameLogo.png");

	arialFont = TTF_OpenFont("Assets/arial.ttf", 25);
}


SDL_Texture* AssetManager::GetTexture(const char* textureFilePath)
{
	SDL_Surface* surface = IMG_Load(textureFilePath);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(GameWindow::GetInstance()->GetRenderer(), surface);
	if (!surface) {
		printf("IMG_Load: %s\n", IMG_GetError());
	}

	SDL_FreeSurface(surface);
	return texture;
}


TTF_Font* AssetManager::GetFont(Font font)
{
	switch (font)
	{
	case AssetManager::Arial:
		return arialFont;
	default:
		std::cout << "Not a valid Font" << std::endl;
		return nullptr;
	}
}

SDL_Texture* AssetManager::GetEmptyCell()
{
	return emptyCell;
}

SDL_Texture* AssetManager::GetGridBackground()
{
	return gridBackground;
}

SDL_Texture* AssetManager::GetBallsBackground()
{
	return ballsBackground;
}

SDL_Texture* AssetManager::GetMainMenuArrow()
{
	return mainMenuArrow;
}

SDL_Texture* AssetManager::GetGameLogo()
{
	return gameLogo;
}



SDL_Color AssetManager::GetTextColor()
{
	return textColor;
}

SDL_Texture* AssetManager::GetFontTexture()
{
	return fontTexture;
}


AssetManager* AssetManager::GetInstance()
{
	if (!instance)
		instance = new AssetManager();
	return instance;
}


void AssetManager::ReleaseInstance()
{
	delete this;
}


std::tuple<SDL_Texture*, Ball::Color> AssetManager::GetRandomEntityTexture()
{
	int index = std::rand() % 4;
	switch (index)
	{
	case 0:
		return std::make_tuple(balls[Ball::Color::Red], Ball::Color::Red);
	case 1:
		return std::make_tuple(balls[Ball::Color::Green], Ball::Color::Green);
	case 2:
		return std::make_tuple(balls[Ball::Color::Blue], Ball::Color::Blue);
	case 3:
		return std::make_tuple(balls[Ball::Color::Purple], Ball::Color::Purple);
	}
}


SDL_Texture* AssetManager::GetSquareTexture(Ball::Color ballColor)
{
	switch (ballColor)
	{
	case Ball::Color::Red:
		return squares[Ball::Color::Red];
	case Ball::Color::Green:
		return squares[Ball::Color::Green];
	case Ball::Color::Blue:
		return squares[Ball::Color::Blue];
	case Ball::Color::Purple:
		return squares[Ball::Color::Purple];
	}
}


SDL_Texture* AssetManager::GetBallHelperTexture(Ball::Color ballColor)
{
	switch (ballColor)
	{
	case Ball::Color::Red:
		return ballHelpers[Ball::Color::Red];
	case Ball::Color::Green:
		return ballHelpers[Ball::Color::Green];
	case Ball::Color::Blue:
		return ballHelpers[Ball::Color::Blue];
	case Ball::Color::Purple:
		return ballHelpers[Ball::Color::Purple];
	default:
		return nullptr;
	}

}


SDL_Texture* AssetManager::GetBallTextureFromColor(Ball::Color ballColor)
{
	switch (ballColor)
	{
	case Ball::Color::Red:
		return balls[Ball::Color::Red];
	case Ball::Color::Green:
		return balls[Ball::Color::Green];
	case Ball::Color::Blue:
		return balls[Ball::Color::Blue];
	case Ball::Color::Purple:
		return balls[Ball::Color::Purple];
	default:
		return nullptr;
	}
}
