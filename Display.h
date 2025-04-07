#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Leaderboard.h" 

class Display {
    sf::Texture backgroundTexture;
    sf::Text leaderboard;
    sf::Sprite backgroundSprite;
    sf::Font main;
    sf::RenderWindow& window;
    
public:
    Display(sf::RenderWindow& _window) : window(_window) {

        if (!font.loadFromFile("fonts/Roboto-Black.ttf")) {
            std::cerr << "Failed to load font\n";
        }
        main.loadFromFile("fonts/Alegreya-Bold.ttf");
    }

    void run(Leaderboard input) {

        backgroundTexture.loadFromFile("Texture/Leaderboard.png");

        backgroundSprite.setTexture(backgroundTexture);

        myleaderboard = input;
        sf::Clock clock;
        while (isOpen) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    isOpen = false;
                else if (event.type == sf::Event::KeyPressed) {
                    switch (event.key.code) {
                    case sf::Keyboard::Return:
                        if (isSelected) {
                            isOpen = false;
                        }
                        break;
                    case sf::Keyboard::Down:
                        if (!isSelected) {
                            isSelected = true;
                        }
                        break;
                    case sf::Keyboard::Up:
                        if (isSelected) {
                            isSelected = false;
                        }
                        break;
                    }
                }
            }

            draw();
        }
    }

private:
    void update(float dt) {
    }

    Leaderboard myleaderboard;


    int selectedButton = 0;
    sf::Event event;


    void draw() {
        window.clear(sf::Color(173, 216, 230));
        window.draw(backgroundSprite);
        
        struct PlayerScore {
            std::string name;
            int score;
        };


      /*  leaderboard.setFont(main);
        leaderboard.setString("Leaderboard");
        leaderboard.setCharacterSize(70);
        leaderboard.setFillColor(sf::Color(100, 95, 95));
        leaderboard.setPosition(820 - leaderboard.getLocalBounds().width / 2, 70);

        window.draw(leaderboard);*/
        myleaderboard.Load_Leaderboard();
        std::vector <Leaderboard::PlayerScore> leaderboardd = myleaderboard.getTopScores();
        int yPosition = 290;
        int xPosition = 320;
        int save = 320;
        int i = 0;
        for (const auto& entry : leaderboardd) {
            sf::Text text;
            
            ++i;
           
            text.setFont(font);
            text.setString(std::to_string(i));
            text.setCharacterSize(50);
            text.setFillColor(sf::Color::White);
            text.setPosition(xPosition - text.getLocalBounds().width / 2, yPosition);
            
            window.draw(text);
            xPosition += 310;
            text.setString(entry.name);
            text.setPosition(680 - text.getLocalBounds().width/2, yPosition);
            window.draw(text);
            xPosition += 600;
            text.setString(std::to_string(entry.score));
            text.setPosition(1150 - text.getLocalBounds().width/2, yPosition);
            window.draw(text);
            xPosition = save ;

            yPosition += 70;
        }


            sf::Color green(101, 170, 176);
      
            sf::RectangleShape button(sf::Vector2f(150.f, 50.f)); 
            button.setPosition(740, 1110);

            sf::Text buttonText;
            buttonText.setFont(main);
            buttonText.setString("Close");
            buttonText.setCharacterSize(50);
            buttonText.setFillColor(isSelected ? green : sf::Color::White);  
            buttonText.setPosition(button.getPosition().x + 20, button.getPosition().y + 5);
            window.draw(buttonText);

        window.display();
    }

    bool isSelected = false;
    bool isOpen = true;

    sf::Font font;
};

