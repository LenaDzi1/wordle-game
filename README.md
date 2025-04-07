# 🎮 Wordle C++ – Word Guessing Game

This is a C++ implementation of the classic **Wordle** game with a graphical interface, modular architecture, and persistent leaderboard. The project uses a menu-based system with category-based word selection, and is built entirely using object-oriented design.

![Gameplay Preview](https://github.com/LenaDzi1/wordle-game/blob/main/img/Wordle.gif)

---

## 🧠 Features

- 🟩 Classic 5-letter word guessing gameplay
- 🎨 Graphical UI (via SFML or similar)
- 🗂 Category-based word generation
- 🏆 Persistent leaderboard with scores
- 🧭 Main menu and help screen navigation
- 📦 Modular class-based structure

---

## 🧱 Class Architecture Overview

### 🎲 Game Logic

- `GameBoard` – manages the state of guesses and results
- `WordGenerator` – generates random words based on selected categories
- `PlayerInput` – handles user input
- `ScoreCounter` – tracks and updates player score

### 🖥 Interface & Rendering

- `Display` – manages the game window and current screen state
- `MainMenu`, `Menu`, `MenuItem`, `TextMenuItem` – UI navigation and rendering
- `Pomoc` – in-game help/instruction screen

### 🏆 Leaderboard System

- `Leaderboard` – reads/writes score data
- `PlayerScore` – struct that holds score info

### 📂 File & Category Handling

- `FileSelector` – loads word lists from category files
- `Entry` – struct that represents an item in a file

![Class Diagram](./full_class_diagram.png)

---



## Requirements

- C++17 or newer
- SFML (Simple and Fast Multimedia Library)
- CMake or Visual Studio (recommended)
