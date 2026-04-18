#include "Player.h"
#include "Game.h"

namespace AppleGame
{
	void InitPlayer(Player& player, sf::Texture& texture)
	{
		// Инициализация состояния игрока
		player.position = { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f };
		player.speed = INITIAL_SPEED;
		player.direction = PlayerDirection::Right;

		// Инициализация спрайта игрока
		player.sprite.setTexture(texture);
		SetSpriteSize(player.sprite, PLAYER_SIZE, PLAYER_SIZE);
		SetSpriteOrigin(player.sprite, 0.f, 0.f);
		player.sprite.setPosition(player.position.x, player.position.y);
	}

	void DrawPlayer(Player& player, sf::RenderWindow& window)
	{
		player.sprite.setPosition(player.position.x, player.position.y);
		window.draw(player.sprite);
	}
	//поворот игрока
	void SetPlayerRotation(Player& player)
	{
		switch (player.direction)
		{
		case PlayerDirection::Right:
			player.sprite.setRotation(0.f);
			break;
		case PlayerDirection::Up:
			player.sprite.setRotation(-90.f);
			break;
		case PlayerDirection::Left:
			player.sprite.setRotation(180.f);
			break;
		case PlayerDirection::Down:
			player.sprite.setRotation(90.f);
			break;
		}
	}
}