#pragma once
#include <string>

namespace AppleGame
{
    const std::string RESOURCES_PATH = "Resources/";
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const float INITIAL_SPEED = 100.f; // Пикселей в секунду
    const float PLAYER_SIZE = 20.f;
    const float ACCELERATION = 20.f; // Пикселей в секунду за яблоко
    const float APPLE_SIZE = 20.f;
    const float PAUSE_LENGTH = 3.f;
    const int NUM_ROCKS = 10;
    const float ROCK_SIZE = 20.f;

    // Режимы игры через битовые маски
    enum class GameMode
    {
        None = 0,
        InfiniteApples = 1 << 0,   // 1 - бесконечные яблоки
        WithAcceleration = 1 << 1, // 2 - с ускорением
        FiftyApples = 1 << 2       // 4 - 50 яблок вместо 20
    };

    // Перегрузка оператора | для GameMode (чтоб можно было комбинировать)
    inline GameMode operator|(GameMode a, GameMode b)
    {
        return static_cast<GameMode>(static_cast<int>(a) | static_cast<int>(b));
    }

    // Перегрузка оператора & для GameMode (чтоб проверять флаги)
    inline bool operator&(GameMode a, GameMode b)
    {
        return (static_cast<int>(a) & static_cast<int>(b)) != 0;
    }
}