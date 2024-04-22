#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <cctype>

using namespace std;

string hiddenWord;
vector<char> guessedLetters;
string alphabet = "abcdefghijklmnopqrstuvwxyz";


string WordsRandom() {
    ifstream file("words.txt");
    vector<string> words;
    string word;
    if (file.is_open()) {
        while (getline(file, word)) {
            for (char& letter : word) {

                if (isalpha(letter)) {
                    int index = alphabet.find(tolower(letter));
                    if (index != string::npos) {
                        index -= 3;
                        if (index < 0)
                        {
                            index = alphabet.size() + index;
                        }
                        letter = alphabet[index];
                    }
                }
            }
            words.push_back(word);
        }
        file.close();
    }
    else {
        cout << "Failed to open file" << endl;
    }
    srand(time(NULL));
    return words[rand() % words.size()];
}

void printWord() {
    for (char letter : hiddenWord) {
        if (find(guessedLetters.begin(), guessedLetters.end(), letter) != guessedLetters.end()) {
            cout << letter << " ";
        }
        else {
            cout << "_ ";
        }
    }
    cout << endl;
}

void GameOverPrint(time_t startTime, int attempts) {
    cout << "Game Over" << endl;
    cout << "Time spent: " << difftime(time(NULL), startTime) << " sec" << endl;
    cout << "Tries: " << attempts << endl;
    cout << "Hidden word: " << hiddenWord << endl;
    cout << "Your letters: ";
    for (char letter : guessedLetters) {
        cout << letter << " ";
    }
    cout << endl;
}

int main() {
    hiddenWord = WordsRandom();
    int attempts = 0;
    time_t startTime = time(NULL);
    bool loop = true;
    while (loop) {
        if (attempts >= 10)
        {
            loop = false;
            cout << "Ran out of attempts" << endl;
            GameOverPrint(startTime, attempts);
            break;
        }
        cout << "Word: ";
        printWord();

        if (find_if(hiddenWord.begin(), hiddenWord.end(), [&](char letter) {
            return find(guessedLetters.begin(), guessedLetters.end(), letter) == guessedLetters.end();
            }) == hiddenWord.end()) {
            cout << "You win!" << endl;
            GameOverPrint(startTime, attempts);
            break;
        }

        cout << "Guess a letter: ";
        char guess;
        cin >> guess;

        if (find(guessedLetters.begin(), guessedLetters.end(), guess) != guessedLetters.end()) {
            cout << "Already attempted letter, try again" << endl;
            continue;
        }

        guessedLetters.push_back(guess);

        if (hiddenWord.find(guess) != string::npos) {
            cout << "Right letter!" << endl;
        }
        else {
            cout << "Wrong letter" << endl;
            attempts++;
        }
    }

    return 0;
}
