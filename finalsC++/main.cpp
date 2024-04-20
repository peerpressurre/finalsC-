#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <filesystem> 
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;

class File {
private:
    std::fstream file;

public:
    File(const std::string& filename, std::ios_base::openmode mode) {
        file.open(filename, mode);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
        }
    }

    ~File() {
        if (file.is_open()) {
            file.close();
        }
    }

    bool isOpen() const {
        return file.is_open();
    }

    void write(const std::string& data) {
        file << data;
    }

    std::string read() {
        std::string content;
        std::string line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        return content;
    }
};

class FileManager {
public:
    static bool fileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }

    static void createWalletFile( std::string& walletName,  Wallet& wallet) {
        std::ofstream file(walletName + ".txt");
        if (file.is_open()) {
            file << "Wallet Name: " << wallet.getName() << "\n\n";
            for (auto& card : wallet.getCards()) {
                file << "Card Number: " << card.getNumber() << "\n";
                file << "Holder Name: " << card.getName() << "\n";
                file << "Balance: " << card.getBalance() << "\n";
                file << "Transactions:\n";
                for (auto& transactionPair : card.getTransactions()) {
                    auto& category = transactionPair.first;
                    auto& transactions = transactionPair.second;

                    file << category << ":\n";

                    for (auto& transaction : transactions) {
                        file << "- " << transaction.getAmount() << "$ " << transaction.getDate() << "\n";
                    }
                }
                file << "\n";
            }
        }
        else {
            cout << "Failed to create wallet: " << walletName << endl;
        }
    }
};


class Wallet {
    string holderName;
    vector<Card> cards;
public:
    Wallet(vector<Card> cardss) : cards(cardss) {};
    string getName() {
        return holderName;
    }

    vector<Card>& getCards() {
        return cards;
    }
};

class Card {
    string number;
    string holderName;
    double balance;
    map<string, vector<Transaction>> transactions;
public:
    Card(string num, string name, double bal) : number(num), holderName(name), balance(bal) {}
    string getNumber() {
        return number;
    }
    string getName() {
        return holderName;
    }
    double getBalance() {
        return balance;
    }
    map<string, vector<Transaction>> getTransactions() {
        return transactions;
    }
    void withdraw(string category, double amount) {
        if (balance >= amount)
        {
            balance -= amount;
            transactions[category].push_back(Transaction(category, amount));
        }
        else
        {
            cout << "Insufficient funds!" << endl;
        }
    }
    void deposit(double amount) {
        balance += amount;
    }
   
};

class Transaction {
private:
    string category;
    double amount;
    Date date;

public:
    Transaction(string cat, double amt) : category(cat), amount(amt) {}
    string getCategory() {
        return category;
    }
    double getAmount() {
        return amount;
    }
    string getDate() {
        return date.formatDateTime();
    }
};

class Date {
    std::chrono::system_clock::time_point timePoint;

public:
    Date() : timePoint(std::chrono::system_clock::now()) {}

    int getYear() const {
        time_t time = chrono::system_clock::to_time_t(timePoint);
        tm* localTime = localtime(&time);
        return localTime->tm_year + 1900; 
    }

    int getMonth() const {
        time_t time = chrono::system_clock::to_time_t(timePoint);
        tm* localTime = localtime(&time);
        return localTime->tm_mon + 1;
    }

    int getDay() const {
        time_t time = chrono::system_clock::to_time_t(timePoint);
        tm* localTime = localtime(&time);
        return localTime->tm_mday; 
    }

    int getHour() const {
        time_t time = chrono::system_clock::to_time_t(timePoint);
        tm* localTime = localtime(&time);
        return localTime->tm_hour;
    }

    int getMinute() const {
        time_t time = chrono::system_clock::to_time_t(timePoint);
        tm* localTime = localtime(&time);
        return localTime->tm_min;
    }

    int getSecond() const {
        time_t time = chrono::system_clock::to_time_t(timePoint);
        tm* localTime = localtime(&time);
        return localTime->tm_sec; 
    }

    string formatDateTime() const {
        time_t time = chrono::system_clock::to_time_t(timePoint);
        tm* localTime = localtime(&time);

        std::ostringstream oss;
        oss << setfill('0');
        oss << setw(2) << localTime->tm_mday << ' '
            << setw(2) << (localTime->tm_mon + 1) << '-'
            << setw(4) << (localTime->tm_year + 1900) << '-'
            << setw(2) << localTime->tm_hour << ':'
            << setw(2) << localTime->tm_min << ':'
            << setw(2) << localTime->tm_sec;

        return oss.str();
    }
};

