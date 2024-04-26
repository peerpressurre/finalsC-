#define _CRT_SECURE_NO_WARNINGS 
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


class Transaction {
private:
    string category;
    double amount;
    string date;

public:
    Transaction(string cat, double amt, const string transactionDate) : category(cat), amount(amt), date(transactionDate) {}
    string getCategory() {
        return category;
    }
    double getAmount() {
        return amount;
    }
    string getDate() {
        return date;
    }
};


class FileManager {
public:
    static bool fileExists(const string& filename) {
        fstream file(filename);
        return file.good();
    }

    static void createWalletFile(string& walletName, Wallet& wallet) {
        fstream file;
        file.open(walletName + ".txt", ios::app);
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
            cout << "Failed to create a wallet: " << walletName << endl;
        }
    }

    static void listWalletInfo(string& walletName) {
        fstream file;
        file.open(walletName + ".txt", ios::in);
        cout << walletName << endl;
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                if (line.find("Card Number:") != string::npos) {
                    cout << line << endl;

                    getline(file, line);
                    cout << line << endl;

                    while (getline(file, line) && line.find("Transactions") == string::npos) {
                        cout << line << endl;
                    }
                    cout << endl;
                }
            }
            file.close();
        }
        else {
            cerr << "Failed to open wallet file: " << walletName << endl;
        }
    }

    static void writeTransactionInfo(string& walletName, map<string, vector<Transaction>> transactions, double newamount) {
        fstream file;
        file.open(walletName + ".txt", ios::app);
        if (!file.is_open()) {
            cout << "Failed to open wallet file: " << walletName + ".txt" << endl;
            return;
        }

        string line;
        while (getline(file, line))
        {
            if (line.find("Transactions") != string::npos)
            {
                for (auto& pair : transactions)
                {
                    for (size_t i = 0; i < pair.second.size(); i++)
                    {
                        file << pair.second[i].getAmount() << "$ " << pair.second[i].getCategory() << " " << pair.second[i].getDate() << endl;
                    }
                }
            }
        }
        updateBalance(walletName, newamount);
    }

    static void updateBalance(string& walletName, double newamount) {
        fstream inFile;
        inFile.open(walletName + ".txt", ios::in);
        if (!inFile.is_open()) {
            cout << "Failed to open wallet file: " << walletName + ".txt" << endl;
            return;
        }

        vector<string> lines;
        string line;
        while (getline(inFile, line)) {
            lines.push_back(line);
        }
        inFile.close();

        fstream outFile(walletName + ".txt");
        if (!outFile.is_open()) {
            cout << "Failed to open wallet file for writing: " << walletName + ".txt" << endl;
            return;
        }

        for (size_t i = 0; i < lines.size(); i++) {
            if (lines[i].find("Balance") != string::npos) {
                outFile << "Balance: " << newBalance << endl;
            }
            else {
                outFile << lines[i] << endl;
            }
        }
        outFile.close();
    }

    static void FillInfoFromFile(string& walletName, Wallet& wallet) {
        fstream file;
        file.open(walletName + ".txt", ios::in);
        if (!file.is_open()) {
            cout << "Failed to open wallet file: " << walletName + ".txt" << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.find("Card Number: ") != string::npos) {
                string cardNumber = line.substr(line.find(":") + 2);
                getline(file, line);
                string holderName = line.substr(line.find(":") + 2);
                getline(file, line);
                double balance = stod(line.substr(line.find(":") + 2));

                Card card(cardNumber, holderName, balance);

                while (getline(file, line)) {
                    if (line.find("Transactions:") != string::npos) {
                        while (getline(file, line) && !line.empty()) {
                            size_t pos = line.find("$");
                            double amount = stod(line.substr(0, pos));
                            size_t pos2 = line.find(" ", pos + 1);
                            string category = line.substr(pos + 1, pos2 - pos - 1);
                            string date = line.substr(pos2 + 1);

                            Transaction transaction(string category, double amount, string date);
                            card.getTransactions()[category].push_back(transaction);
                        }
                    }
                }

                wallet.addCard(card);
            }
        }

        file.close();
    }
};


class Card {
    string number;
    string holderName;
    double balance;
    map<string, vector<Transaction>> transactions;
    string owningWallet;
    FileManager fileManager;
public:
    Card(string num, string name, double bal, FileMAnager fileManager)  : number(num), holderName(name), balance(bal), fileManager(fm) {}
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
    void withdraw(string category, double amount, string transactionDate, string walletName) {
        if (balance >= amount) {
            balance -= amount;
            transactions[category].push_back(Transaction(category, amount, transactionDate));
        }
        else {
            cout << "Insufficient funds!" << endl;
        }
    }

    void deposit(double amount) {
        balance += amount;
        fileManager.updateBalance(owningWallet, balance);
    }

    void printMenu() {
        cout << "1 - Card top-up" << endl;
        cout << "2 - Make a transaction" << endl;
    }

};


class Wallet {
    string holderName;
    vector<Card> cards;
    FileMAnager fileManager;
public:
    Wallet(string name, vector<Card> cardss, FileMAnager fileManager) : holderName(name), cards(cardss), fileManager(fm) {};
    string getName() {
        return holderName;
    }
    vector<Card>& getCards() {
        return cards;
    }
    void addCard(Card& card) {
        cards.push_back(card);
    }
    void printMenu() {
        cout << "1 - Add card" << endl;
        cout << "2 - Remove card" << endl;
        cout << "3 - View card";
    }
    void printCards() {
         
    }
};



int main() {
    string walletName;
    FileManager fm;
    cout << "Enter the name of your wallet: ";
    getline(std::cin, walletName);
    vector<Card> cards;
    Wallet wallet;
    if (!FileManager::fileExists(walletName + ".txt")) {
        cout << "No such wallet found" << endl;
        cout << "Creating a new wallet.." << endl;
        cout << "Enter wallet holder name -> ";
        string wholderName;
        getline(cin, wholderName);
        wallet(wholderName, cards, fm);
        cout << "name: " << walletName << endl;
        bool exitLoop = true;
        char ans;
        int counter = 0;
        while (exitLoop)
        {
            cout << "Add card? y/n -> ";
            cin >> ans;
            cin.ignore();
            if (ans == 'y')
            {
                string number;
                string holderName;
                double balance;
                while (true)
                {
                    cout << "Enter card number (16 digits) -> ";
                    getline(cin, number);
                    number.erase(remove_if(number.begin(), number.end(), std::isspace), number.end());
                    if (number.size() != 16) {
                        cout << "!Card number must consist of 16 digits" << endl;
                    }
                    else {
                        exitLoop = false;
                        break;
                    }
                }
                cout << "Enter card holder name -> ";
                getline(cin, holderName);
                cout << "Enter current balance -> ";
                cin >> balance;
                Card card(number, holderName, balance);
                cards.push_back(card);
                
            }
        }  
        Wallet wallet(walletName, cards);
        FileManager::createWalletFile(walletName, wallet);
    }
    else {
        fm.FillInfoFromFile(walletName, Wallet & wallet);
        fm.listWalletInfo(walletName);
        wallet.printMenu();
        
    }

  
    return 0;
}
    /*/ Creating a debit card
    Card debitCard("123456789", "John Doe", 1000.0);
    cards.push_back(debitCard);

    // Creating a credit card
    Card creditCard("987654321", "Jane Smith", 500.0);
    cards.push_back(creditCard);

    // Creating the wallet
    Wallet wallet(cards);

    // Saving wallet data to file
    FileManager::createWalletFile(walletName, wallet);

    // Sample functions
    // You can add more functions for managing finances here

    // Example: Add expense
    addExpense(wallet, "groceries", 50.0);

    // Example: Print report for the day
    printDailyReport(wallet);

    // Example: Print top 3 expenses of the month
    printTopExpensesOfMonth(wallet);

    // Example: Print top 3 expense categories of the week
    printTopExpenseCategoriesOfWeek(wallet);

    return 0;
}

void addExpense(Wallet& wallet, const std::string& category, double amount) {
    // Assuming the expense is for the first card in the wallet
    if (!wallet.getCards().empty()) {
        // Withdraw the amount from the first card with the given category
        wallet.getCards()[0].withdraw(category, amount);
    }
}

void printDailyReport(const Wallet& wallet) {
    std::cout << "Daily Report:\n";
    // Implement logic to print daily report
}

void printTopExpensesOfMonth(const Wallet& wallet) {
    std::cout << "Top 3 Expenses of the Month:\n";
    // Implement logic to print top expenses of the month
}

void printTopExpenseCategoriesOfWeek(const Wallet& wallet) {
    std::cout << "Top 3 Expense Categories of the Week:\n";
    // Implement logic to print top expense categories of the week
}*/