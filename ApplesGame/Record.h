#pragma once
#include <string>
#include <map>
#include <vector>

namespace AppleGame
{
    struct Record
    {
        std::string name;
        int score;
    };

    // Генерация вымышленных имён для таблицы рекордов
    std::vector<std::string> GenerateFakeNames();

    // Создание начальной таблицы рекордов
    std::map<int, Record> CreateInitialLeaderboard();

    // Обновление таблицы рекордов с результатами игрока
    void UpdateLeaderboard(std::map<int, Record>& leaderboard, const std::string& playerName, int playerScore);

    // Получение отсортированного вектора записей из map (ручная сортировка)
    std::vector<Record> GetSortedLeaderboard(const std::map<int, Record>& leaderboard);

    // Отображение таблицы рекордов
    std::string GetLeaderboardString(const std::map<int, Record>& leaderboard, const std::string& playerName);
}