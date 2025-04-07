#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Menu.h"
#include "Leaderboard.h"
#include "Display.h"
#include "Pomoc.h"

using namespace sf;

/////////////////////////// MENUITEM /////////////////////


class MenuItem {
public:
    virtual ~MenuItem() = default;
    virtual void draw(RenderWindow& window) = 0;
    virtual void select() = 0;
    virtual void deselect() = 0;
};


/////////////////// TEXT MENU ITEM ////////////////


class TextMenuItem : public MenuItem {
private:
    Text text;
    Color selectedColor;
    Color defaultColor;

public:

    TextMenuItem(const std::string& str, const Font& font, unsigned int size, Color defaultColor, Color selectedColor, Vector2f position)
        : defaultColor(defaultColor), selectedColor(selectedColor) {
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(defaultColor);
        text.setPosition(position);
        text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);

    }

    void draw(RenderWindow& window) override {
        window.draw(text);
    }

    void select() override {
        text.setFillColor(selectedColor);
    }

    void deselect() override {
        text.setFillColor(defaultColor);
    }
};



//////////////////////// MAIN MENU ///////////////////////////////



class MainMenu {
private:
    std::vector<MenuItem*> menuItems;
    int MainMenuSelected;
    Font font;
    static const int Max_main_menu = 4;
    Leaderboard leaderboard;

public:

    void start() {

        RenderWindow MENU(VideoMode(1600, 1250), "Wordle", sf::Style::Titlebar | sf::Style::Close);

        RectangleShape background;
        background.setSize(Vector2f(1600, 1250));
        Texture Maintexture;
        Maintexture.loadFromFile("Texture/Mainmenu.png");
        background.setTexture(&Maintexture);

        RectangleShape Game_background;
        Game_background.setSize(Vector2f(1600, 1250));
        Texture Back_texture;
        Back_texture.loadFromFile("Texture/Category.png");
        Game_background.setTexture(&Back_texture);

        RectangleShape Leaderboard_background;
        Leaderboard_background.setSize(Vector2f(1600, 1250));
        Texture Optiontexture;
        Optiontexture.loadFromFile("Texture/Leaderboard.png");
        Leaderboard_background.setTexture(&Optiontexture);

        RectangleShape Help_background;
        Help_background.setSize(Vector2f(1600, 1250));
        Texture Abouttexture;
        Abouttexture.loadFromFile("Texture/Howplay.png");
        Help_background.setTexture(&Abouttexture);

        while (MENU.isOpen()) {
            Event event;
            while (MENU.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    MENU.close();
                }
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Up) {
                        MoveUp();
                        break;
                    }
                    if (event.key.code == Keyboard::Down) {
                        MoveDown();
                        break;
                    }
                    if (event.key.code == Keyboard::Return) {
                        int selectedItem = getPressedItem();

                        switch (selectedItem) {
                        case 0: {
                            while (selectedItem == 0) {
                                Menu menu(MENU);
                                menu.runApplication();

                                selectedItem = 4;
                                break;
                            }
                            break;
                            }
                              

                        case 1: // Leaderboard
                        {
                            leaderboard.Load_Leaderboard();

                            Display display(MENU);
                            display.run(leaderboard);
                            selectedItem = 4;
                            break;
                        }
                         

                        case 2: // Pomoc

                        {

                            Pomoc pomoc(MENU);
                            pomoc.run();
                    
                            selectedItem = 4;
                            break;
                        }
                        
                        case 3: // Wyjscie
                            MENU.close();
                            break;

                        case 4: {

                           MENU.clear();
                           MENU.draw(background);
                           draw(MENU);
                           MENU.display();
                           break;
                          }
                        }
                    }
                }
            }
            MENU.clear();
            MENU.draw(background);
            draw(MENU);
            MENU.display();
        }
    }


    MainMenu(float width, float height);
    ~MainMenu();
    void draw(RenderWindow& window);
    void MoveUp();
    void MoveDown();
    int getPressedItem() const { return MainMenuSelected; }
};



MainMenu::MainMenu(float width, float height) {
    if (!font.loadFromFile("fonts/Alegreya-Bold.ttf")) {
        std::cout << "No font here \n";
    }

    Color green(101, 170, 176);

    menuItems.push_back(new TextMenuItem("New Game", font, 70, Color::White, green, Vector2f(1130, 493)));
    menuItems.push_back(new TextMenuItem("Leaderboard", font, 70, Color::White, green, Vector2f(1130, 695)));
    menuItems.push_back(new TextMenuItem("Help", font, 70, Color::White, green, Vector2f(1130, 895)));
    menuItems.push_back(new TextMenuItem("Exit", font, 70, Color::White, green, Vector2f(1130, 1080)));

    MainMenuSelected = 0;
    menuItems[MainMenuSelected]->select();
}

MainMenu::~MainMenu() {
    for (auto item : menuItems) {
        delete item;
    }
}

void MainMenu::draw(RenderWindow& window) {
    for (auto item : menuItems) {
        item->draw(window);
    }
}

void MainMenu::MoveUp() {
    if (MainMenuSelected - 1 >= 0) {
        menuItems[MainMenuSelected]->deselect();
        MainMenuSelected--;
        menuItems[MainMenuSelected]->select();
    }
}

void MainMenu::MoveDown() {
    if (MainMenuSelected + 1 < Max_main_menu) {
        menuItems[MainMenuSelected]->deselect();
        MainMenuSelected++;
        menuItems[MainMenuSelected]->select();
    }
}



