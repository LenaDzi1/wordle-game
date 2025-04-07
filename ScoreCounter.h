#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <string>
#include <vector>
#include <chrono>
#include <numeric>
#include <regex>
#include <filesystem>
#include <mutex>
#include <thread>
#include <ranges>


class ScoreCounter {
    std::vector<int> coefficients = { 10, 5, -5 };
public:
    void playTurn(std::vector<int> points) {
        std::vector<int> transformed(points.size());
        std::ranges::transform(points, coefficients, transformed.begin(), std::multiplies<>());

        int sum = std::accumulate(transformed.begin(), transformed.end(), 0);

        addPoints(sum);
    }

    ScoreCounter() : score(0) {}

    void addPoints(int points) {
        std::lock_guard<std::mutex> guard(score_mutex);
        score += points;
        
    }

    int getScore() const {
        std::lock_guard<std::mutex> guard(score_mutex);
        return score;
    }

private:
    int score;
    mutable std::mutex score_mutex;
};