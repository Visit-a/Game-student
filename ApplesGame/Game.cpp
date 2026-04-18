#include "Game.h"
#include <cassert>
#include <iostream>

namespace AppleGame
{
    // Функция установки режима игры
    void SetGameMode(Game& game, GameMode mode)
    {
        game.gameMode = mode;

        // Определяем кол-во яблок в зависимости от режима
        if (CheckGameMode(game, GameMode::FiftyApples))
        {
            game.numApples = 50;
            std::cout << "Mode: 50 apples\n";
        }
        else
        {
            game.numApples = 20;
            std::cout << "Mode: 20 apples\n";
        }

        if (CheckGameMode(game, GameMode::InfiniteApples))
        {
            std::cout << "Mode: Infinite apples\n";
        }
        else
        {
            std::cout << "Mode: Finite apples\n";
        }

        if (CheckGameMode(game, GameMode::WithAcceleration))
        {
            std::cout << "Mode: With acceleration\n";
        }
        else
        {
            std::cout << "Mode: Without acceleration\n";
        }
    }

    // Проверка наличия флага режима
    bool CheckGameMode(const Game& game, GameMode mode)
    {
        return game.gameMode & mode;
    }

    void RestartGame(Game& game)
    {
        InitPlayer(game.player, game.playerTexture);

        // ДЗ: очищаем старую память если есть
        if (game.apples != nullptr)
        {
            delete[] game.apples;
            game.apples = nullptr;
        }

        // ДЗ: выделяем новую память под массив яблок
        game.apples = new Apple[game.numApples];

        // Инициализируем все яблоки
        for (int i = 0; i < game.numApples; ++i)
        {
            InitApple(game.apples[i], game.appleTexture);
        }

        // Инициализация камней (как раньше)
        for (int i = 0; i < NUM_ROCKS; ++i)
        {
            InitRock(game.rocks[i], game.rockTexture);
        }

        game.numEatenApples = 0;
        game.isGameFinished = false;
        game.timeSinceGameFinish = 0;
    }

    void InitGame(Game& game)
    {
        // Загружаем текстуры
        assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "Player.png"));
        assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "Apple.png"));
        assert(game.rockTexture.loadFromFile(RESOURCES_PATH + "Rock.png"));

        // Загружаем звуки
        if (game.eatSoundBuffer.loadFromFile(RESOURCES_PATH + "AppleEat.wav"))
        {
            game.eatSound.setBuffer(game.eatSoundBuffer);
        }
        if (game.collisionSoundBuffer.loadFromFile(RESOURCES_PATH + "Death.wav"))
        {
            game.collisionSound.setBuffer(game.collisionSoundBuffer);
        }

        // Настраиваем фон
        game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        game.background.setFillColor(sf::Color::Black);
        game.background.setPosition(0.f, 0.f);

        // спрашиваем у пользователя режим игры
        std::cout << "=== GAME MODE SELECTION ===\n";
        std::cout << "1. 20 apples + acceleration\n";
        std::cout << "2. 20 apples, NO acceleration\n";
        std::cout << "3. 50 apples + acceleration\n";
        std::cout << "4. Infinite apples + acceleration\n";
        std::cout << "5. Infinite apples, NO acceleration\n";
        std::cout << "Choose mode (1-5): ";

        int choice;
        std::cin >> choice;

        GameMode mode;
        switch (choice)
        {
        case 1:
            mode = GameMode::WithAcceleration;
            break;
        case 2:
            mode = GameMode::None;
            break;
        case 3:
            mode = GameMode::FiftyApples | GameMode::WithAcceleration;
            break;
        case 4:
            mode = GameMode::InfiniteApples | GameMode::WithAcceleration;
            break;
        case 5:
            mode = GameMode::InfiniteApples;
            break;
        default:
            std::cout << "Wrong choice, using default mode 1\n";
            mode = GameMode::WithAcceleration;
            break;
        }

        SetGameMode(game, mode);
        RestartGame(game);
    }

    void UpdateGame(Game& game, float deltaTime)
    {
        // Обновление состояния игры
        if (!game.isGameFinished)
        {
            // Обработка ввода с клавиатуры
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                game.player.direction = PlayerDirection::Right;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                game.player.direction = PlayerDirection::Up;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                game.player.direction = PlayerDirection::Left;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                game.player.direction = PlayerDirection::Down;
            }

            // Двигаем игрока
            switch (game.player.direction)
            {
            case PlayerDirection::Right:
                game.player.position.x += game.player.speed * deltaTime;
                break;
            case PlayerDirection::Up:
                game.player.position.y -= game.player.speed * deltaTime;
                break;
            case PlayerDirection::Left:
                game.player.position.x -= game.player.speed * deltaTime;
                break;
            case PlayerDirection::Down:
                game.player.position.y += game.player.speed * deltaTime;
                break;
            }

            // Поворачиваем спрайт игрока
            SetPlayerRotation(game.player);

            // Проверяем столкновения с яблоками
            for (int i = 0; i < game.numApples; ++i)
            {
                if (IsCirclesCollide(game.player.position, PLAYER_SIZE / 2.f,
                    game.apples[i].position, APPLE_SIZE / 2.f))
                {
                    // проверяем режим бесконечных яблок
                    if (CheckGameMode(game, GameMode::InfiniteApples))
                    {
                        // Перемещаем яблоко в новое место
                        game.apples[i].position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
                        game.apples[i].sprite.setPosition(game.apples[i].position.x, game.apples[i].position.y);
                    }
                    else
                    {
                        // Прячем яблоко за экран (типа съели)
                        game.apples[i].position = { -100.f, -100.f };
                        game.apples[i].sprite.setPosition(game.apples[i].position.x, game.apples[i].position.y);
                    }

                    ++game.numEatenApples;
                    std::cout << "Apples eaten: " << game.numEatenApples << "\n";

                    // проверяем режим ускорения
                    if (CheckGameMode(game, GameMode::WithAcceleration))
                    {
                        game.player.speed += ACCELERATION;
                        std::cout << "Speed increased to: " << game.player.speed << "\n";
                    }

                    game.eatSound.play();
                }
            }

            // Проверяем столкновения с камнями
            for (int i = 0; i < NUM_ROCKS; ++i)
            {
                if (IsCirclesCollide(game.player.position, PLAYER_SIZE / 2.f,
                    game.rocks[i].position, ROCK_SIZE / 2.f))
                {
                    game.isGameFinished = true;
                    game.timeSinceGameFinish = 0.f;
                    game.collisionSound.play();
                }
            }

            // Проверяем выход за границы экрана
            if (game.player.position.x - PLAYER_SIZE / 2.f < 0.f ||
                game.player.position.x + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
                game.player.position.y - PLAYER_SIZE / 2.f < 0.f ||
                game.player.position.y + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
            {
                game.isGameFinished = true;
                game.timeSinceGameFinish = 0.f;
                game.collisionSound.play();
            }
        }
        else
        {
            // Игра закончена, ждем паузу перед рестартом
            if (game.timeSinceGameFinish <= PAUSE_LENGTH)
            {
                game.timeSinceGameFinish += deltaTime;
                game.background.setFillColor(sf::Color::Red);
            }
            else
            {
                // Рестарт игры
                game.background.setFillColor(sf::Color::Black);
                RestartGame(game);
            }
        }
    }

    void DrawGame(Game& game, sf::RenderWindow& window)
    {
        window.draw(game.background);

        // Рисуем игрока
        DrawPlayer(game.player, window);

        // Рисуем только видимые яблоки
        for (int i = 0; i < game.numApples; ++i)
        {
            // Проверяем что яблоко на экране (костыль конечно, но работает)
            if (game.apples[i].position.x >= 0 && game.apples[i].position.y >= 0)
            {
                game.apples[i].sprite.setPosition(game.apples[i].position.x, game.apples[i].position.y);
                window.draw(game.apples[i].sprite);
            }
        }

        // Рисуем камни
        for (int i = 0; i < NUM_ROCKS; ++i)
        {
            game.rocks[i].sprite.setPosition(game.rocks[i].position.x, game.rocks[i].position.y);
            window.draw(game.rocks[i].sprite);
        }
    }

    void DeinializeGame(Game& game)
    {
        //очищаем память!
        if (game.apples != nullptr)
        {
            delete[] game.apples;
            game.apples = nullptr;
        }
        std::cout << "Game deinitialized, memory cleaned\n";
    }
}