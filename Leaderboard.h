#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <string>
#include <vector>
#include <chrono>

#include <regex>
#include <filesystem>
#include <mutex>
#include <thread>



class Leaderboard {
   
public:

    struct PlayerScore {
        std::string name;
        int score;
    };

    PlayerScore score;

    void Load_Leaderboard () {
        loadScores();
    }

    void addScore(const std::string& name, int score) {
        scores.push_back({ name, score });
        saveScores();
    }

    std::vector<PlayerScore> getTopScores(int topN = 10) {
        std::sort(scores.begin(), scores.end(), [](const PlayerScore& a, const PlayerScore& b) {
            return b.score < a.score;
            });

        if (scores.size() > topN) {
            return std::vector<PlayerScore>(scores.begin(), scores.begin() + topN);
        }
        else {
            return scores;
        }

    }

    bool chceckName(const std::string& imie) {
        std::regex wzorzec("^[A-Z•∆ £—”åèØ][a-zπÊÍ≥ÒÛúüø]+$");
        return std::regex_match(imie, wzorzec);
    }

    void viewLeaderboard() {
        std::vector<PlayerScore> leaderboard = getTopScores();
        for (const auto& playerScore : leaderboard) {
            std::cout << playerScore.name << ": " << playerScore.score << std::endl;
        }
        saveScores();
    }

private:

    std::string filename = "txt/leaderboard.txt";
    std::vector<PlayerScore> scores;

    void saveScores() {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& playerScore : scores) {
                file << playerScore.name << " " << playerScore.score << std::endl;
            }
            file.close();
        }
        else {
            std::cerr << "Unable to open file for writing: " << filename << std::endl;
        }
    }

    void loadScores() {
        std::ifstream file(filename);
        if (file.is_open()) {
            scores.clear();
            std::string name;
            int score;
            while (file >> name >> score) {
                scores.push_back({ name, score });
            }
            file.close();
        }
        else {
            std::cerr << "Unable to open file for reading: " << filename << std::endl;
        }
    }
};