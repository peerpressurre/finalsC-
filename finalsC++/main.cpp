#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;



string secretWord;

vector<char> guessedLetters;

// ������� ��� ��������� ���������� ���
void printGameStats(time_t startTime, int attempts) {
    cout << "��� ���������!" << endl;
    cout << "��� ���: " << difftime(time(NULL), startTime) << " ������" << endl;
    cout << "ʳ������ �����: " << attempts << endl;
    cout << "������ �����: " << secretWord << endl;
    cout << "˳���� ������: ";
    for (char letter : guessedLetters) {
        cout << letter << " ";
    }
    cout << endl;
}

// ������� ��� ��������� ����� ����� (� ��������� � ����������� �������)
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

// ������� ��� ������ ����������� �����
string chooseRandomWord() {
    vector<string> words = { "��������", "���������", "�������������", "�����", "������" };
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
        cout << "�����: ";
        printWordState();

        // �������� �� ����� ��� �������
        if (find_if(secretWord.begin(), secretWord.end(), [&](char letter) {
            return find(guessedLetters.begin(), guessedLetters.end(), letter) == guessedLetters.end();
            }) == secretWord.end()) {
            cout << "�� �������!" << endl;
            printGameStats(startTime, attempts);
            break;
        }

        // �������� ����� �� ������
        cout << "������ �����: ";
        char guess;
        cin >> guess;

        // �������� �� ���� ��� ������� �� �����
        if (find(guessedLetters.begin(), guessedLetters.end(), guess) != guessedLetters.end()) {
            cout << "�� ��� ������� �� �����. ��������� �� ���." << endl;
            continue;
        }

        guessedLetters.push_back(guess);

        // �������� �� ����� � � ���������� ����
        if (secretWord.find(guess) != string::npos) {
            cout << "�� ������� �����!" << endl;
        }
        else {
            cout << "�� ����� �� � �������� �����." << endl;
            attempts++;
        }
    }

    return 0;
}