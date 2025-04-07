#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <thread>
#include <chrono>

class Pomoc {

    std::string text1 = "Guess the word by entering the appropriate letters.\n For each letter you get points:\n";
    std::string text2 = ": -5 points\n\n: +5 points\n\n: +10 points";
    std::string text3 = "To complete the game you have to guess all the letters.\nRemember - you have one more attempt than the length\nof the word!";
    std::string text4 = "Good luck!";

public:
    Pomoc(sf::RenderWindow& window)
        : window(window)
    {

        std::string backgroundPath = "Texture/Howplay.png";
        std::string fontPath = "fonts/Alegreya-Bold.ttf";
       
        //unsigned int fontSize = 70;

        if (!backgroundTexture.loadFromFile(backgroundPath)) {
            throw std::runtime_error("Nie mo¿na za³adowaæ t³a");
        }
        backgroundSprite.setTexture(backgroundTexture);

        if (!font.loadFromFile(fontPath)) {
            throw std::runtime_error("Nie mo¿na za³adowaæ czcionki");
        }
        helpText.setFont(font);
       
        helpText.setCharacterSize(60);
        helpText.setFillColor(sf::Color(100, 95, 95));

    }
    bool isSelected = false;
    bool isOpen = true;

    void run() {



        while (isOpen) {
            for (int i = 0; i < 4; i++) {

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

            window.clear();
            draw(i);
            window.display();
        }
        }
    }

private:
    sf::RectangleShape grid;
    const int cellSize = 80;
    const int outlineThickness = 2;

    void drawgrid() {
       
        grid.setSize(sf::Vector2f(cellSize, cellSize));
        grid.setOutlineThickness(outlineThickness);
        grid.setOutlineColor(sf::Color::Black);
        window.draw(grid);
    }


    void draw(int i) {
        int select = i+1;
        window.draw(backgroundSprite);
        
        helpText.setString(text1);
        helpText.setPosition(50,50);
        
        window.draw(helpText);

        helpText.setString(text2);
        helpText.setPosition(800, 240);
        window.draw(helpText);
        helpText.setString(text3);
        helpText.setCharacterSize(60);
        helpText.setPosition(50, 700);
        window.draw(helpText);
        helpText.setString(text4);
        helpText.setPosition(670, 950);
        window.draw(helpText);
        grid.setPosition(700, 250);
        if (select == 4) grid.setFillColor(sf::Color(240, 240, 240));
        else grid.setFillColor(sf::Color(255, 204, 204));
        drawgrid();
        grid.setPosition(700, 410);
        if(select == 1 || select ==4) grid.setFillColor(sf::Color(240, 240, 240));
        else grid.setFillColor(sf::Color(255, 255, 204));
        drawgrid();
        grid.setPosition(700, 570);
        if (select == 3)  grid.setFillColor(sf::Color(204, 255, 204));
        else grid.setFillColor(sf::Color(240, 240, 240));
        drawgrid();
        std::this_thread::sleep_for(std::chrono::milliseconds(150));

        sf::Color green(101, 170, 176);
        sf::Text buttonText;
        buttonText.setFont(font);
        buttonText.setString("Close");
        buttonText.setCharacterSize(50);
        buttonText.setFillColor(isSelected ? green : sf::Color::White);
        buttonText.setPosition(760, 1120);
        window.draw(buttonText);
    }

    sf::RenderWindow& window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Font font;
    sf::Text helpText;
};
