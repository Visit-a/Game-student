#include "Rock.h"

namespace AppleGame
{
	void InitRock(Rock& rock, sf::Texture& texture)
	{
		rock.position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);

		rock.sprite.setTexture(texture);
		SetSpriteSize(rock.sprite, ROCK_SIZE, ROCK_SIZE);
		SetSpriteOrigin(rock.sprite, 0.f, 0.f);
		rock.sprite.setPosition(rock.position.x, rock.position.y);
	}
}