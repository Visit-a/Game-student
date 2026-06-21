// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Game.h"
#include <ctime>

int main()
{
    using namespace AppleGame;

    // Инициализация рандома
    int seed = (int)time(nullptr);
    srand(seed);

    // Создаем окно
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Apples game!");

    // Создаем игру
    Game game;
    InitGame(game);  // внутри спросит режим и имя

    // Часы для дельты времени
    sf::Clock gameClock;
    float lastTime = gameClock.getElapsedTime().asSeconds();

    // Главный игровой цикл
    while (window.isOpen())
    {
        // Считаем дельту времени
        float currentTime = gameClock.getElapsedTime().asSeconds();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Обрабатываем события
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
                break;
            }
        }

        // Обновляем игру
        UpdateGame(game, deltaTime);

        // Рисуем
        window.clear();
        DrawGame(game, window);
        window.display();
    }

    // Очищаем память перед выходом
    DeinializeGame(game);

    return 0;
}