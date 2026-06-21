#include "Record.h"

namespace AppleGame
{
    std::vector<std::string> GenerateFakeNames()
    {
        std::vector<std::string> names = {
            "Alice", "Bob", "Carol", "Dave", "Eve",
            "Frank", "Grace", "Henry", "Ivy", "Jack",
            "Kate", "Leo", "Mia", "Noah", "Olivia"
        };
        return names;
    }

    std::map<int, Record> CreateInitialLeaderboard()
    {
        std::map<int, Record> leaderboard;
        std::vector<std::string> names = GenerateFakeNames();

        int keys[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        for (int i = 0; i < 10 && i < (int)names.size(); ++i)
        {
            int score = 20 + (rand() % 80);  // rand() доступен через GameMain.cpp
            leaderboard[keys[i]] = { names[i], score };
        }

        return leaderboard;
    }

    void UpdateLeaderboard(std::map<int, Record>& leaderboard, const std::string& playerName, int playerScore)
    {
        int playerKey = -1;
        for (auto& pair : leaderboard)
        {
            if (pair.second.name == playerName)
            {
                playerKey = pair.first;
                break;
            }
        }

        if (playerKey != -1)
        {
            leaderboard[playerKey].score = playerScore;
        }
        else
        {
            int newKey = 1;
            for (auto& pair : leaderboard)
            {
                if (pair.first >= newKey)
                {
                    newKey = pair.first + 1;
                }
            }
            leaderboard[newKey] = { playerName, playerScore };
        }
    }

    std::vector<Record> GetSortedLeaderboard(const std::map<int, Record>& leaderboard)
    {
        std::vector<Record> result;

        for (auto& pair : leaderboard)
        {
            result.push_back(pair.second);
        }

        // Пузырьковая сортировка (ручная)
        int n = (int)result.size();
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (result[j].score < result[j + 1].score)
                {
                    Record temp = result[j];
                    result[j] = result[j + 1];
                    result[j + 1] = temp;
                }
            }
        }

        return result;
    }

    std::string GetLeaderboardString(const std::map<int, Record>& leaderboard, const std::string& playerName)
    {
        std::string result = "\n===== LEADERBOARD =====\n";

        std::vector<Record> sorted = GetSortedLeaderboard(leaderboard);

        int count = 0;
        for (const Record& record : sorted)
        {
            if (count >= 10) break;

            ++count;
            std::string marker = (record.name == playerName) ? " *" : "";
            result += std::to_string(count) + ". " + record.name + " ...... " + std::to_string(record.score) + marker + "\n";
        }

        result += "======================\n";
        return result;
    }
}