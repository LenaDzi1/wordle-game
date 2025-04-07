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


class PlayerInput {

public:

    std::string convertLargeToSmall(const std::string& input_text) {

        std::regex pattern("[A-Z]");

        std::string result;
        std::sregex_iterator currentMatch(input_text.begin(), input_text.end(), pattern);
        std::sregex_iterator lastMatch;

        size_t lastPos = 0;

        while (currentMatch != lastMatch) {
            std::smatch match = *currentMatch;

            result.append(input_text, lastPos, match.position() - lastPos);

            result += std::tolower(match.str().front());

            lastPos = match.position() + match.length();

            ++currentMatch;
        }
        result.append(input_text, lastPos, std::string::npos);

        return result;
    }

    std::string getPlayerGuess(std::string input) {

    
        std::string guess = input;
        
        std::string converted_text = convertLargeToSmall(guess);

        return converted_text;
    }
};