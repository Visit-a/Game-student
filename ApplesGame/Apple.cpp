#include "Apple.h"
namespace AppleGame
{
	void InitApple(Apple& apple, sf::Texture& texture)
	{
		apple.position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);

		apple.sprite.setTexture(texture);
		SetSpriteSize(apple.sprite, APPLE_SIZE, APPLE_SIZE);
		SetSpriteOrigin(apple.sprite, 0.f, 0.f);
		apple.sprite.setPosition(apple.position.x, apple.position.y);
	}
}