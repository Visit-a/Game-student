#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Constants.h"
#include "Math.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"

namespace AppleGame
{
    struct Game
    {
        Player player;

        // динамический массив яблок вместо статического
        Apple* apples = nullptr;  // указатель на массив
        int numApples = 0;        // текущее кол-во яблок

        Rock rocks[NUM_ROCKS];

        // Глобальные данные игры
        int numEatenApples = 0;
        bool isGameFinished = false;
        float timeSinceGameFinish = 0.f;
        sf::RectangleShape background;

        // режим игры через битовую маску
        GameMode gameMode = GameMode::None;

        // Ресурсы
        sf::Texture playerTexture;
        sf::Texture appleTexture;
        sf::Texture rockTexture;

        // Звуки
        sf::SoundBuffer eatSoundBuffer;
        sf::SoundBuffer collisionSoundBuffer;
        sf::Sound eatSound;
        sf::Sound collisionSound;
    };

    void RestartGame(Game& game);
    void InitGame(Game& game);
    void UpdateGame(Game& game, float deltaTime);
    void DrawGame(Game& game, sf::RenderWindow& window);
    void DeinializeGame(Game& game);

    // Вспомогательные функции для режимов
    void SetGameMode(Game& game, GameMode mode);
    bool CheckGameMode(const Game& game, GameMode mode);
}