#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Math.h"

namespace AppleGame
{
	struct Rock
	{
		Position2D position;
		sf::Sprite sprite;
	};

	void InitRock(Rock& rock, sf::Texture& texture);
}