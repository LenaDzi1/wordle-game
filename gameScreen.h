#include <SFML/Graphics.hpp>
#include <SFML/System/Thread.hpp>
#include <vector>
#include <string>
#include <regex>
#include "Display.h"
#include "ScoreCounter.h"
#include "PlayerInput.h"
#include "GameBoard.h"
#include <thread>
#include <chrono>




class GameScreen {

public:

    sf::Mutex mutex;
    std::atomic<bool> running;
    bool renderFlag;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread renderThread;
    std::thread gameThread;

    GameScreen(int wordLength, std::string ategory, sf::RenderWindow& window);
    void run(std::string word);

    void showEndOverlay(); 
   void handleTextInput(sf::RenderWindow& window, sf::Text& text, std::string& playerName);
   
   sf::Texture backgroundTexture;
  
   sf::Sprite backgroundSprite;
  
   bool isValidChar(char c);

private:


    sf::RectangleShape overlay;
    sf::Text endScoreText;
    sf::Text inputPrompt;
    sf::Text inputTextBox;
    sf::Text text;
    std::string playerName;
    void processEvents();
    void update();
    void render();
    void handlePlayerInput(sf::Keyboard::Key key);
    void drawGrid();
    void DrawScore();
    void drawAttempt();
    void DrawCategory();

    Leaderboard leaderboard;
    
    ScoreCounter scoreCounter;
    GameBoard board;
    PlayerInput Input;
 
    int wordLength;
    int rows;
    int currentRow = 0;
    int currentCol = 0;
    const int cellSize = 80;
    const int outlineThickness = 2;

    const int padding = 5;
    const int windowWidth = 1600;
    const int windowHeight = 1250;
    const int gridStartX = (windowWidth - (wordLength * (cellSize + padding) - padding)) / 2;
    const int gridStartY = (windowHeight - (rows * (cellSize + padding) - padding)) / 2;
    std::string category;
    sf::RenderWindow& window;
    sf::Font font;
 
    std::vector<std::vector<sf::RectangleShape>> grid;
    std::vector<std::vector<sf::Text>> letters;
    std::string currentWord;
    std::string guess;

    bool isOpen = true;
};

GameScreen::GameScreen(int wordLength, std::string ategory, sf::RenderWindow& window) : wordLength(wordLength), rows(wordLength + 1), window(window), category(ategory) {
    if (!font.loadFromFile("fonts/Alegreya-Bold.ttf")) {
        throw std::runtime_error("Failed to load font");
    }
    

    grid.resize(rows, std::vector<sf::RectangleShape>(wordLength));
    letters.resize(rows, std::vector<sf::Text>(wordLength));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < wordLength; ++j) {
            grid[i][j].setSize(sf::Vector2f(cellSize, cellSize));
            grid[i][j].setPosition(gridStartX + j * (cellSize + padding), gridStartY + i * (cellSize + padding));
            grid[i][j].setFillColor(sf::Color(240, 240, 240));
            grid[i][j].setOutlineThickness(outlineThickness);
            grid[i][j].setOutlineColor(sf::Color::Black);

            letters[i][j].setFont(font);
            letters[i][j].setCharacterSize(60);
            letters[i][j].setFillColor(sf::Color::Black);
            letters[i][j].setPosition(gridStartX + j * (cellSize + padding) + 16, gridStartY + i * (cellSize + padding) + 5);
        }
    }

}

void GameScreen::run(std::string word) {
    
    backgroundTexture.loadFromFile("Texture/Gameplay.png");
   
    backgroundSprite.setTexture(backgroundTexture);

    guess = word;

    board.set_attempts(word.size());
    while (isOpen) {
        processEvents();
        update();
        render();
        
    }
}

void GameScreen::processEvents() {
    sf::Event event;
  
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            isOpen = false;
        }
        else if (event.type == sf::Event::KeyPressed) {
            handlePlayerInput(event.key.code);
        }
    }
}

void GameScreen::update() {}

void GameScreen::render() {
    window.clear();
    window.draw(backgroundSprite);
    drawGrid();
    DrawScore();
    drawAttempt();
    DrawCategory();


    window.display();
}

void GameScreen::handlePlayerInput(sf::Keyboard::Key key) {
    if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z && currentCol < wordLength) {
        char letter = 'A' + key - sf::Keyboard::A;
        currentWord += letter;
        letters[currentRow][currentCol].setString(letter);
        ++currentCol;
    }
    if (key == sf::Keyboard::Enter && currentCol == wordLength) {
        
        std::vector <int> points = { 0,0,0 };
        currentWord = Input.convertLargeToSmall(currentWord);
        for (int i = 0; i < wordLength; i++) {
            if (currentWord[i] == guess[i]) {
                grid[currentRow][i].setFillColor(sf::Color(204, 255, 204));
                points[0] += 1;

                drawGrid();
                window.display();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            else {
                bool foundInSecret = false;
                for (int n = 0; n < wordLength; n++) {
                    if (currentWord[i] == guess[n]) {
                        foundInSecret = true;
                        break;
                    }
                }
                if (foundInSecret) {
                    grid[currentRow][i].setFillColor(sf::Color(255, 255, 204));
                    points[1] += 1;

                    drawGrid();
                    window.display();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                 
                }
                else {
                
                    grid[currentRow][i].setFillColor(sf::Color(255, 204, 204));
                    points[2] += 1;
                    drawGrid();
                    window.display();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));

                }
            }
        }
        scoreCounter.playTurn(points);
        board.count_attempts();
        
        
        if (board.getRemainingAttempts() == 0 || currentWord == guess) {
            scoreCounter.addPoints(board.getRemainingAttempts() * 100);
            update();
            render();
         
               showEndOverlay();
        }

        currentWord = "";
        currentCol = 0;
        ++currentRow;
        
    }
    else if (key == sf::Keyboard::BackSpace && currentCol > 0) {
        --currentCol;
        currentWord.pop_back();
        letters[currentRow][currentCol].setString("");
    }
  
    
}

void GameScreen::drawGrid() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < wordLength; ++j) {
            window.draw(grid[i][j]);
            window.draw(letters[i][j]);
        }
    }
}

void GameScreen::DrawScore() {
    sf::Text scoreboard;
    scoreboard.setFont(font);
    scoreboard.setCharacterSize(70);
    scoreboard.setFillColor(sf::Color(100, 95, 95));
    scoreboard.setPosition(50, 500);
    scoreboard.setString("Score:" );
    window.draw(scoreboard);
    scoreboard.setFont(font);
    scoreboard.setString('\n'+ std::to_string(scoreCounter.getScore()));
    scoreboard.setPosition(50, 500);
    window.draw(scoreboard);
}

void GameScreen::DrawCategory() {
    sf::Text Category;
    Category.setFont(font);
    Category.setCharacterSize(70);
    Category.setFillColor(sf::Color(101, 170, 176));
    
    Category.setString(category);
    sf::FloatRect textRect = Category.getLocalBounds();

    Category.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    Category.setPosition(810, 100);
    window.draw(Category);
}

void GameScreen::drawAttempt() {
    sf::Text Attempt;
    Attempt.setFont(font);
    Attempt.setCharacterSize(70);
    Attempt.setFillColor(sf::Color(100, 95, 95));
    Attempt.setString("Attempts ");
    Attempt.setPosition(1230, 500);
    window.draw(Attempt);
    Attempt.setFont(font);
    Attempt.setString(std::to_string(board.getRemainingAttempts()));
    Attempt.setPosition(1390, 595);
    window.draw(Attempt);
    Attempt.setFont(font);
    Attempt.setString("\nleft:");
    Attempt.setPosition(1230, 500);
    window.draw(Attempt);
}


void GameScreen::showEndOverlay() {

  

    overlay.setSize(sf::Vector2f(windowWidth / 2, windowHeight / 2));
    overlay.setFillColor(sf::Color(128, 128, 128, 200)); 
    overlay.setPosition(windowWidth / 4, windowHeight / 4);

    endScoreText.setFont(font);
    endScoreText.setCharacterSize(50);
    endScoreText.setFillColor(sf::Color::White);
    endScoreText.setPosition((windowWidth - endScoreText.getLocalBounds().width) / 2 - 300,
        (windowHeight/2 - endScoreText.getLocalBounds().height) / 2 +20);
    std::string message= "Game over. Score: " + std::to_string(scoreCounter.getScore()) + '\n' + "Do you want to save your score?\n";
    endScoreText.setString(message);
    window.draw(endScoreText);
    

   
    //////////////////////////
    sf::Text button1;
    sf::Text button2;
    button1.setFont(font);
    button2.setFont(font);
    button1.setCharacterSize(50);
    button2.setCharacterSize(50);
    button1.setString("Yes");
    button2.setString("No");

    button1.setPosition(
        (window.getSize().x - button1.getLocalBounds().width) / 2,
        (window.getSize().y / 2 + 90)
    );
    button2.setPosition(
        (window.getSize().x - button2.getLocalBounds().width) / 2,
        (window.getSize().y / 2 + 180)
    );

    sf::Color normalColor = sf::Color::White;
    sf::Color selectedColor = sf::Color::Yellow;

    int selectedButton = 0;
    
    
  

    window.clear();
    window.draw(backgroundSprite);
    drawGrid();
    DrawScore();
    drawAttempt();
    DrawCategory();
    window.draw(overlay);
    window.draw(endScoreText);
    window.draw(button1);
    window.draw(button2);
    
    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    texture.update(window);
    sf::Sprite sprite(texture);

    window.draw(sprite);
    window.display();

    while (isOpen) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                isOpen = false;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down) {
                    selectedButton = 1 - selectedButton;
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    if (selectedButton == 0) {
                      
                        sf::RectangleShape textfile;
                        textfile.setSize(sf::Vector2f(500, 70));
                        textfile.setFillColor(sf::Color(240, 240, 240));
                        textfile.setPosition((window.getSize().x - textfile.getLocalBounds().width) / 2, 600);

                        window.draw(textfile);
                        window.draw(button1);
                        window.draw(button2);
                        window.display();

                        texture.create(window.getSize().x, window.getSize().y);
                        texture.update(window);
                        sf::Sprite sprite(texture);


                            text.setFont(font);
                            text.setFillColor(sf::Color::Black);
                            text.setCharacterSize(45);
                            text.setPosition((window.getSize().x - textfile.getLocalBounds().width) / 2, 600);

                            sf::Text zapis;
                            std::string zapis1 = "Enter your name: ";
                            zapis.setFont(font);
                            zapis.setCharacterSize(50);
                            zapis.setFillColor(sf::Color::White);
                            zapis.setPosition(630, 530);
                            zapis.setString(zapis1);

                            std::string playerName;

                            while (isOpen) {
                                handleTextInput(window, text, playerName);
                                window.draw(sprite);
                                window.draw(zapis);
                                window.draw(textfile);
                                window.draw(text);
                                window.display();
                            }

                    }
                    else {
                        isOpen = false;
                    }
                }

            }
            window.draw(sprite);
            window.draw(button1);
            window.draw(button2);
            window.display();
        }

        if (selectedButton == 0) {
            button1.setFillColor(selectedColor);
            button2.setFillColor(normalColor);
        }
        else {
            button1.setFillColor(normalColor);
            button2.setFillColor(selectedColor);
        }

    }
   
}

bool GameScreen::isValidChar(char c) {
    std::string charStr(1, c);
    std::regex pattern("[A-Za-z]");

    return std::regex_match(charStr, pattern);
}


void GameScreen::handleTextInput(sf::RenderWindow& window, sf::Text& text, std::string& playerName) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            isOpen = false;
                    }
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b') { 
                   if (!playerName.empty()) {
                             playerName.pop_back();
                       }
                  }
            else if (event.text.unicode < 128) { 
                if (playerName.size() < 20 && isValidChar(event.text.unicode)) {
                playerName += static_cast<char>(event.text.unicode);
                
                }
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
 
                if (leaderboard.chceckName(playerName)) {
               
                    leaderboard.Load_Leaderboard();
                    leaderboard.addScore(playerName, scoreCounter.getScore());
                 
                    Display display(window);
                    display.run(leaderboard);

                    isOpen = false;
                   
                }
                else {
                   // std::cout << "Niepoprawne imie. Sprobuj ponownie." << std::endl;
                    playerName.clear();
                    text.setString(playerName);
                }
            }
        }
        text.setString(playerName);
    }
}
