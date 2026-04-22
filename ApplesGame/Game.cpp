#include "Game.h"
#include <cassert>
#include <iostream>

namespace AppleGame
{
    void RestartGame(Game& game)
    {
        InitPlayer(game.player, game.playerTexture);

        // Очищаем старую память
        if (game.apples != nullptr)
        {
            delete[] game.apples;
            game.apples = nullptr;
        }

        // Определяем количество яблок в зависимости от режима
        if (game.modeRandomApples)
        {
            // Случайное количество яблок от 10 до 60
            game.numApples = 10 + (rand() % 51);
            std::cout << "Random apples count: " << game.numApples << "\n";
        }
        else if (game.mode50WithAccel)
        {
            game.numApples = 50;
        }
        else
        {
            game.numApples = 20;
        }

        game.applesRemaining = game.numApples;
        game.numEatenApples = 0;

        // Выделяем память под массив яблок
        game.apples = new Apple[game.numApples];
        
        // Инициализация яблок
        for (int i = 0; i < game.numApples; ++i)
        {
            InitApple(game.apples[i], game.appleTexture);
        }

        // Инициализация камней
        for (int i = 0; i < NUM_ROCKS; ++i)
        {
            InitRock(game.rocks[i], game.rockTexture);
        }

        game.isGameFinished = false;
        game.finishState = GameFinishState::None;
        game.timeSinceGameFinish = 0.f;
        game.background.setFillColor(sf::Color::Black);
        
        // Выводим информацию о выбранном режиме
        if (game.modeRandomApples)
            std::cout << "Mode: Random apples (" << game.numApples << ") with acceleration\n";
        else if (game.mode20WithAccel)
            std::cout << "Mode: 20 apples with acceleration\n";
        else if (game.mode20NoAccel)
            std::cout << "Mode: 20 apples without acceleration\n";
        else if (game.mode50WithAccel)
            std::cout << "Mode: 50 apples with acceleration\n";
    }

    void InitGame(Game& game)
    {
        assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "Player.png"));
        assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "Apple.png"));
        assert(game.rockTexture.loadFromFile(RESOURCES_PATH + "Rock.png"));

        // Загружаем шрифт для текста победы/поражения
        if (!game.font.loadFromFile(RESOURCES_PATH + "arial.ttf"))
        {
            // Если шрифт не загрузился, используем дефолтный
            std::cout << "Warning: Font not found, using default\n";
        }
        
        game.finishText.setFont(game.font);
        game.finishText.setCharacterSize(48);
        game.finishText.setFillColor(sf::Color::White);
        game.finishText.setStyle(sf::Text::Bold);

        if (game.eatSoundBuffer.loadFromFile(RESOURCES_PATH + "AppleEat.wav"))
        {
            game.eatSound.setBuffer(game.eatSoundBuffer);
        }
        if (game.collisionSoundBuffer.loadFromFile(RESOURCES_PATH + "Death.wav"))
        {
            game.collisionSound.setBuffer(game.collisionSoundBuffer);
        }

        game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        game.background.setFillColor(sf::Color::Black);
        game.background.setPosition(0.f, 0.f);

        // Выбор режима игры пользователем
        std::cout << "\n=== SELECT GAME MODE ===\n";
        std::cout << "1. 20 apples with acceleration\n";
        std::cout << "2. 20 apples without acceleration\n";
        std::cout << "3. 50 apples with acceleration\n";
        std::cout << "4. Random apples count (10-60) with acceleration\n";
        std::cout << "Enter choice (1-4): ";
        
        int choice;
        std::cin >> choice;
        
        // Сбрасываем все режимы
        game.mode20WithAccel = false;
        game.mode20NoAccel = false;
        game.mode50WithAccel = false;
        game.modeRandomApples = false;
        
        // Устанавливаем выбранный режим
        switch (choice)
        {
        case 1:
            game.mode20WithAccel = true;
            break;
        case 2:
            game.mode20NoAccel = true;
            break;
        case 3:
            game.mode50WithAccel = true;
            break;
        case 4:
            game.modeRandomApples = true;
            break;
        default:
            std::cout << "Wrong choice, using default mode 1\n";
            game.mode20WithAccel = true;
            break;
        }

        RestartGame(game);
    }

    void UpdateGame(Game& game, float deltaTime)
    {
        // Обновление состояния игры
        if (!game.isGameFinished)
        {
            // Обработка ввода
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

            // Обновление состояния игрока
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

            // Поиск столкновений игрока с яблоками
            for (int i = 0; i < game.numApples; ++i)
            {
                // Проверяем только видимые яблоки
                if (game.apples[i].position.x >= 0 && game.apples[i].position.y >= 0)
                {
                    if (IsCirclesCollide(game.player.position, PLAYER_SIZE / 2.f,
                        game.apples[i].position, APPLE_SIZE / 2.f))
                    {
                        // Перемещаем яблоко за экран (съедено)
                        game.apples[i].position = { -100.f, -100.f };
                        game.apples[i].sprite.setPosition(game.apples[i].position.x, game.apples[i].position.y);
                        
                        ++game.numEatenApples;
                        --game.applesRemaining;
                        
                        std::cout << "Apples eaten: " << game.numEatenApples 
                                  << ", remaining: " << game.applesRemaining << "\n";
                        
                        // Проверяем, нужно ли ускорение
                        if (game.mode20WithAccel || game.mode50WithAccel || game.modeRandomApples)
                        {
                            game.player.speed += ACCELERATION;
                            std::cout << "Speed: " << game.player.speed << "\n";
                        }

                        game.eatSound.play();
                        
                        // Проверяем победу - все яблоки съедены
                        if (game.applesRemaining <= 0)
                        {
                            game.isGameFinished = true;
                            game.finishState = GameFinishState::Victory;
                            game.timeSinceGameFinish = 0.f;
                            std::cout << "=== VICTORY! All apples collected! ===\n";
                        }
                    }
                }
            }

            // Если игра уже завершена победой, дальше не проверяем столкновения
            if (game.isGameFinished)
            {
                return;
            }

            // Поиск столкновений игрока с камнями
            for (int i = 0; i < NUM_ROCKS; ++i)
            {
                if (IsCirclesCollide(game.player.position, PLAYER_SIZE / 2.f,
                    game.rocks[i].position, ROCK_SIZE / 2.f))
                {
                    game.isGameFinished = true;
                    game.finishState = GameFinishState::Defeat;
                    game.timeSinceGameFinish = 0.f;
                    game.collisionSound.play();
                    std::cout << "=== DEFEAT! Hit a rock! ===\n";
                    return;
                }
            }

            // Проверка столкновения с границами экрана
            if (game.player.position.x - PLAYER_SIZE / 2.f < 0.f || 
                game.player.position.x + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
                game.player.position.y - PLAYER_SIZE / 2.f < 0.f || 
                game.player.position.y + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
            {
                game.isGameFinished = true;
                game.finishState = GameFinishState::Defeat;
                game.timeSinceGameFinish = 0.f;
                game.collisionSound.play();
                std::cout << "=== DEFEAT! Out of bounds! ===\n";
            }
        }
        else
        {
            // Игра закончена, ждем паузу перед рестартом
            if (game.timeSinceGameFinish <= PAUSE_LENGTH)
            {
                game.timeSinceGameFinish += deltaTime;
                
                // Разный цвет фона для победы и поражения
                if (game.finishState == GameFinishState::Victory)
                {
                    game.background.setFillColor(sf::Color::Green);
                }
                else if (game.finishState == GameFinishState::Defeat)
                {
                    game.background.setFillColor(sf::Color::Red);
                }
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
        
        // Рисуем текст победы/поражения
        if (game.isGameFinished)
        {
            std::string textStr;
            if (game.finishState == GameFinishState::Victory)
            {
                textStr = "VICTORY!\nAll " + std::to_string(game.numApples) + " apples collected!";
                game.finishText.setFillColor(sf::Color::Yellow);
            }
            else if (game.finishState == GameFinishState::Defeat)
            {
                textStr = "DEFEAT!\nApples collected: " + std::to_string(game.numEatenApples) + 
                          "/" + std::to_string(game.numApples);
                game.finishText.setFillColor(sf::Color::White);
            }
            
            game.finishText.setString(textStr);
            
            // Центрируем текст
            sf::FloatRect textRect = game.finishText.getLocalBounds();
            game.finishText.setOrigin(textRect.left + textRect.width / 2.0f,
                                       textRect.top + textRect.height / 2.0f);
            game.finishText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
            
            window.draw(game.finishText);
        }
    }


    //чистка памяти
    void DeinializeGame(Game& game)
    {
        if (game.apples != nullptr)
        {
            delete[] game.apples;
            game.apples = nullptr;
        }
        std::cout << "Game deinitialized\n";
    }
}