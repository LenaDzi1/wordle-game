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

class GameBoard {
private:
    std::string secretWord;
    std::vector<std::string> guessedWords;
    int attempts;

public:

    int wordLength;

    void set_attempts(int wordLength) {
        attempts = wordLength + 1;
    }

    void setSecretWord(const std::string& word) {
        secretWord = word;
    }

    bool checkGuess(const std::string& guess) {
        return guess == secretWord;
    }

    int getRemainingAttempts() const {

        return attempts;
    }

    void count_attempts() {
      
            attempts = attempts - 1;

        

    }

    std::string getSecretWord() const { 
        return secretWord;
    }

    int getWordLength() const {
        return wordLength;
    }


};