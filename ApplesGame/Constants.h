#pragma once
#include <string>

namespace AppleGame
{
    const std::string RESOURCES_PATH = "Resources/";
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const float INITIAL_SPEED = 100.f; // Пикселей в секунду
    const float PLAYER_SIZE = 20.f;
    const float ACCELERATION = 20.f; // Пикселей в секунду
    const float APPLE_SIZE = 20.f;
    const float PAUSE_LENGTH = 3.f;
    const int NUM_ROCKS = 10;
    const float ROCK_SIZE = 20.f;

    // НЕ ОПРЕДЕЛЯЕМ GameMode здесь, он будет только в Game.h
}