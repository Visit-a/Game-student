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
    // Состояния завершения игры
    enum class GameFinishState
    {
        None,           // Игра продолжается
        Victory,        // Победа 
        Defeat          // Поражение 
    };

    struct Game
    {
        Player player;

        // Динамический массив яблок
        Apple* apples = nullptr;
        int numApples = 0;
        int applesRemaining = 0;  // Сколько яблок ещё не съедено

        Rock rocks[NUM_ROCKS];

        // Глобальные данные игры
        int numEatenApples = 0;
        bool isGameFinished = false;
        GameFinishState finishState = GameFinishState::None;
        float timeSinceGameFinish = 0.f;
        sf::RectangleShape background;

        // Три булевых переменных для режимов игры
        bool mode20WithAccel = true;   // Режим 1: 20 яблок с ускорением
        bool mode20NoAccel = false;    // Режим 2: 20 яблок без ускорения
        bool mode50WithAccel = false;  // Режим 3: 50 яблок с ускорением
        bool modeRandomApples = false; // Режим 4: случайное количество яблок

        // Ресурсы
        sf::Texture playerTexture;
        sf::Texture appleTexture;
        sf::Texture rockTexture;

        // Текст для отображения победы/поражения
        sf::Font font;
        sf::Text finishText;

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
}