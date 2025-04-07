#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <string>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <regex>
#include <filesystem>
#include <mutex>
#include <thread>

using namespace std;
using namespace std::filesystem;


class FileSelector {
public:
    int choice;
 
    struct Entry {
        int number;
        std::string word;
    };


    std::string fileName;
    std::vector<Entry> entries;


    void openFile(int choice) {

        switch (choice) {
        case 1:
            fileName = "txt/Geografia.txt";
            break;
        case 2:
            fileName = "txt/Muzyka.txt";
            break;
        case 3:
            fileName = "txt/Filmy.txt";
            break;
        case 4:
            fileName = "txt/Zwierzeta.txt";
            break;
        case 5:
            fileName = "txt/Historia.txt";
            break;
        case 6:
            fileName = "txt/Gry.txt";
            break;
        default:
            throw std::runtime_error("Invalid selection");
        }

        // Check if the file exists using filesystem
        if (!exists(fileName)) {
            throw std::runtime_error("File does not exist");
        }

        std::ifstream file(fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }

        std::cout << "Twoja kategoria: " << fileName.substr(0, fileName.size() - 4) << '\n';
    }

    void readFromFile(const std::string& filename, std::vector<Entry>& entries) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open the file." << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            Entry entry;

            std::getline(iss, token, ';');
            entry.number = std::stoi(token);

            std::getline(iss, token, ';');
            entry.word = token;

            entries.push_back(entry);
        }

        file.close();
    }


    std::vector<std::string> getWordsWithNumber(const std::vector<Entry>& entries, int number) {
        std::vector<std::string> words;
        for (const auto& entry : entries) {
            if (entry.number == number) {
                words.push_back(entry.word);
            }
        }
        return words;
    }


};