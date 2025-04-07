#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <regex>
#include <sstream>
#include <iostream>
#include "WordGenerator.h"

class Menu {
public:


    bool isValidChar(char c) {
        std::string charStr(1, c);
        std::regex pattern("[4-9]");
        return std::regex_match(charStr, pattern);
    }

    Menu(sf::RenderWindow& window)
        : font(), textButtons(6), selectedIndex(0), categorySelected(false), userInput(""), window(window), generator(window) {

        if (!font.loadFromFile("fonts/Alegreya-Bold.ttf")) {
    
        }
        
      
        for (int i = 0; i < 6; ++i) {
            sf::Text& buttonText = textButtons[i];
            buttonText.setFont(font);
            buttonText.setCharacterSize(79); 
            buttonText.setFillColor(sf::Color::White);
            buttonText.setStyle(sf::Text::Bold);
            buttonText.setString(buttonLabels[i]);
        }
        sf::Color green(101, 170, 176);
        textButtons[selectedIndex].setFillColor(green);

        categoryPrompt.setFont(font);
        categoryPrompt.setCharacterSize(70);
        categoryPrompt.setFillColor(sf::Color(100,95,95));
        categoryPrompt.setString("Select category");

        inputText.setFont(font);
        inputText.setCharacterSize(80);
        inputText.setFillColor(green);

        selectedCategoryText.setFont(font);
        selectedCategoryText.setCharacterSize(70);
        selectedCategoryText.setFillColor(sf::Color(100, 95, 95));

        inputPrompt.setFont(font);
        inputPrompt.setCharacterSize(70);
        inputPrompt.setFillColor(sf::Color(100, 95, 95));
    }

    void draw(sf::RenderWindow& window) {
        float windowWidth = window.getSize().x;
        float windowHeight = window.getSize().y;

            categoryPrompt.setPosition((windowWidth - categoryPrompt.getGlobalBounds().width) / 2, 50);
            window.draw(categoryPrompt);

            float startX = 590;
            float startY = 250; 

            for (int i = 0; i < 6; ++i) {
                float x = (i % 2) * 660.f; 
                float y = (i / 2) * 260.f; 
                textButtons[i].setPosition(startX + x - 100.f, startY + y);

                sf::FloatRect textRect = textButtons[i].getLocalBounds();
                textButtons[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top ); 
                textButtons[i].setPosition(startX + x - 100.f, startY + y );
                
                window.draw(textButtons[i]);
            }
      
        if (categorySelected) {
       
            selectedCategoryText.setPosition((windowWidth - selectedCategoryText.getGlobalBounds().width) / 2, 920);
            window.draw(selectedCategoryText);

            inputPrompt.setString("Type word length (");
            inputPrompt.setPosition(350, 1050);
            window.draw(inputPrompt);
            inputPrompt.setString("4-9");
            inputPrompt.setPosition(910, 1050);
            inputPrompt.setFont(font);
            window.draw(inputPrompt);
            inputPrompt.setString("):");
            inputPrompt.setPosition(1020, 1050);
            inputPrompt.setFont(font);
            window.draw(inputPrompt);

            inputText.setPosition(1170, 1050);
            window.draw(inputText);
        }
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
  
            if (event.type == sf::Event::KeyPressed) {
    
                textButtons[selectedIndex].setFillColor(sf::Color::White);

                switch (event.key.code) {
                case sf::Keyboard::Up:
                    selectedIndex = (selectedIndex - 2 + textButtons.size()) % textButtons.size();
                    break;
                case sf::Keyboard::Down:
                    selectedIndex = (selectedIndex + 2) % textButtons.size();
                    break;
                case sf::Keyboard::Left:
                    selectedIndex = (selectedIndex - 1 + textButtons.size()) % textButtons.size();
                    break;
                case sf::Keyboard::Right:
                    selectedIndex = (selectedIndex + 1) % textButtons.size();
                    break;
                case sf::Keyboard::Return:
                    generator.choice = selectedIndex + 1;
                    selectedCategory = buttonLabels[selectedIndex];
                    generator.category = selectedCategory;
                    categorySelected = true;
                    selectedCategoryText.setString("Your category: " + selectedCategory);
                   
                  
                   


                    break;
                }
                sf::Color green(101, 170, 176);
                textButtons[selectedIndex].setFillColor(green);
            }
  
        if (categorySelected) {
            if (event.type == sf::Event::TextEntered) {
                if (isValidChar(static_cast<char>(event.text.unicode)) && userInput.size() < 1) {
                    userInput += static_cast<char>(event.text.unicode);
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (!userInput.empty()) {
                        userInput.pop_back();
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    if (!userInput.empty()) {
                        generator.wordLength = std::stoi(userInput) ;
                        
                        runda.setFont(font);
                        runda.setCharacterSize(28);
                        runda.setFillColor(sf::Color::White);
                       window.display();
                        
                     
                        generator.New_Game();

                       

                        isOpen = false;
                    }
                }
            }
            inputText.setString(userInput);
        }
    }

    void runApplication() {
           
        sf::Texture backgroundTexture;
        backgroundTexture.loadFromFile("Texture/Category.png");
        sf::Sprite backgroundSprite;
        backgroundSprite.setTexture(backgroundTexture);

        while (isOpen) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    isOpen = false;
                handleEvent(event, window);
            }

            window.clear();
            window.draw(backgroundSprite);
            draw(window);
            window.display();
        }
    }

private:
  
    sf::Font font;
    std::vector<sf::Text> textButtons;
    int selectedIndex;
    const std::vector<std::string> buttonLabels = {
            "Geography", "Music", "Film", "Animals", "History", "Games"
    };
 
    bool categorySelected;
    std::string selectedCategory;
    std::string userInput;
    sf::Text categoryPrompt;
    sf::Text selectedCategoryText;
    sf::Text inputPrompt;
    sf::Text inputText;
    sf::Text runda;
    sf::RenderWindow& window;
    WordGenerator generator;
    

    bool isOpen = true;
};