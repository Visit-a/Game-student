#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Constants.h"
#include "Math.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"
#include "Record.h"
#include <cstdint>
#include <map>

namespace AppleGame
{
    // Режимы игры через битовые маски
    enum class GameMode : uint32_t
    {
        None = 0,
        WithAcceleration = 1 << 0,     // 1 - с ускорением
        FiftyApples = 1 << 1,          // 2 - 50 яблок
        RandomApples = 1 << 2          // 4 - случайное количество яблок
    };

    // Состояния завершения игры
    enum class GameFinishState
    {
        None,           // Игра продолжается
        Victory,        // Победа (все яблоки собраны)
        Defeat          // Поражение (столкновение с камнем или границей)
    };

    // Вспомогательные функции для работы с битовыми масками
    inline bool HasGameModeFlag(GameMode mode, GameMode flag)
    {
        return (static_cast<uint32_t>(mode) & static_cast<uint32_t>(flag)) != 0;
    }

    inline GameMode CombineGameModes(GameMode a, GameMode b)
    {
        return static_cast<GameMode>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

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

        // Режим игры через битовую маску
        GameMode gameMode = GameMode::None;

        // Таблица рекордов
        std::map<int, Record> leaderboard;
        std::string playerName = "Player"; // Имя игрока

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
    void SetGameMode(Game& game, GameMode mode);
}