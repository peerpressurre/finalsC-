#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;



string hiddenWord;
vector<char> guessedLetters;

string WordsRandom() {
    vector<string> words = { "kettle", "phone", "medicine", "house", "lighter", "computer", "water", "cruise"};
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
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    hiddenWord = WordsRandom();
    int attempts = 0;
    time_t startTime = time(NULL);

    while (true) {
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

        // Ïåðåâ³ðêà ÷è ë³òåðà º â çàãàäàíîìó ñëîâ³
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
