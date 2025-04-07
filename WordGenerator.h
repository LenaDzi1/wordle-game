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
#include <SFML/Graphics.hpp>
#include "FileSelector.h"
#include "GameBoard.h"
#include "gameScreen.h"


class WordGenerator {
   
    FileSelector selector;
    GameBoard gameBoard;
   // Menu menu;
  
public:
    int wordLength;
    std::vector<std::string> list;
    std::string category;
    sf::RenderWindow& window;

    int choice;

    WordGenerator(sf::RenderWindow& window) : window(window) {}

    void New_Game() {

        selector.openFile(choice);
        selector.readFromFile(selector.fileName, selector.entries);

        list = selector.getWordsWithNumber(selector.entries, wordLength);
        for (int i = 0; i < list.size(); i++) {
            std::cout << list[i] << '\n';
        }
        gameBoard.set_attempts(wordLength);
        gameBoard.setSecretWord(generateRandomWord());

        GameScreen game(wordLength, category, window);

        game.run(gameBoard.getSecretWord());
    }

    std::string generateRandomWord() {

       

        //// RWG ////
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> distr(0, list.size() - 1);
        ///////////////

        int randomIndex = distr(gen);


        return list[randomIndex];


    }

};