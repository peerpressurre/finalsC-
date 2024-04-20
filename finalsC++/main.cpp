#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;



string secretWord;

vector<char> guessedLetters;

// Функція для виведення статистики гри
void printGameStats(time_t startTime, int attempts) {
    cout << "Гра завершена!" << endl;
    cout << "Час гри: " << difftime(time(NULL), startTime) << " секунд" << endl;
    cout << "Кількість спроб: " << attempts << endl;
    cout << "Шукане слово: " << secretWord << endl;
    cout << "Літери гравця: ";
    for (char letter : guessedLetters) {
        cout << letter << " ";
    }
    cout << endl;
}

// Функція для виведення стану слова (з відкритими і прихованими літерами)
void printWordState() {
    for (char letter : secretWord) {
        if (find(guessedLetters.begin(), guessedLetters.end(), letter) != guessedLetters.end()) {
            cout << letter << " ";
        }
        else {
            cout << "_ ";
        }
    }
    cout << endl;
}

// Функція для вибору випадкового слова
string chooseRandomWord() {
    vector<string> words = { "шибаниця", "автомобіль", "програмування", "вазон", "помідор" };
    srand(time(NULL));
    return words[rand() % words.size()];
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    //setlocale(LC_CTYPE, "ukr");
    secretWord = chooseRandomWord();
    int attempts = 0;
    time_t startTime = time(NULL);

    while (true) {
        cout << "Слово: ";
        printWordState();

        // Перевірка чи слово вже вгадане
        if (find_if(secretWord.begin(), secretWord.end(), [&](char letter) {
            return find(guessedLetters.begin(), guessedLetters.end(), letter) == guessedLetters.end();
            }) == secretWord.end()) {
            cout << "Ви виграли!" << endl;
            printGameStats(startTime, attempts);
            break;
        }

        // Введення літери від гравця
        cout << "Введіть літеру: ";
        char guess;
        cin >> guess;

        // Перевірка чи була вже введена ця літера
        if (find(guessedLetters.begin(), guessedLetters.end(), guess) != guessedLetters.end()) {
            cout << "Ви вже вводили цю літеру. Спробуйте ще раз." << endl;
            continue;
        }

        guessedLetters.push_back(guess);

        // Перевірка чи літера є в загаданому слові
        if (secretWord.find(guess) != string::npos) {
            cout << "Ви вгадали літеру!" << endl;
        }
        else {
            cout << "Ця літера не є частиною слова." << endl;
            attempts++;
        }
    }

    return 0;
}