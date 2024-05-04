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
#include <cstdlib>
#include <stdio.h>
#include <intrin.h>
#include <windows.h>

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

class Wallet;
class Card;

class FileManager {
public:
    static bool fileExists(const string& filename);
    static void writeWalletFile(string& walletName, Wallet& wallet);
    static void listWalletInfo(string& walletName);
    static void writeTransactionInfo(string& walletName, map<string, vector<Transaction>> transactions, double newamount);
    static void updateBalance(string& walletName, double newamount, string cardNum);
    static void FillInfoFromFile(string walletName, Wallet& wallet);
};

class Card {
    string number;
    string holderName;
    double balance;
    map<string, vector<Transaction>> transactions;
    string owningWallet;
    
public:
    static FileManager fileManager;
    Card() : number(""), holderName(""), balance(0.0) {};\
    Card(string num, string name, double bal);
    string getNumber();
    string getName();
    double getBalance();
    void setBalance(double amount);
    map<string, vector<Transaction>> getTransactions();
    void withdraw(string category, double amount, string transactionDate);
    void deposit(string walletName, double amount);
    void printMenu();
    void printInfo();
};

class Wallet {
    string holderName;
    vector<Card> cards;
public:
    static FileManager fileManager;
    Wallet() : holderName("") {};
    Wallet(string name, vector<Card> cardss);
    string getName();
    vector<Card>& getCards();
    void setName(string name);
    void setCards(vector<Card>);
    void addCard(Card& card);
    void printMenu();
    void printCards();
    void printCardInfo(int index);
    Card getCardByIndex(int index);
    void printInfo(string walletname);
};


// FileManager
bool FileManager::fileExists(const string& filename) {
    fstream file(filename);
    return file.good();
}

void FileManager::writeWalletFile(string& walletName, Wallet& wallet) {
    fstream file;
    file.open(walletName + ".txt", ios::out);
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
        file.close();
    }
    else {
        cout << "Failed to create a wallet: " << walletName << endl;
    }
}

void FileManager::listWalletInfo(string& walletName) {
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

void FileManager::writeTransactionInfo(string& walletName, map<string, vector<Transaction>> transactions, double newamount) {
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
        //updateBalance(walletName, newamount, cardnum);
        file.close();
}

void FileManager::updateBalance(string& walletName, double newAmount, string cardNum) {
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

    fstream outFile(walletName + ".txt", ios::out); 

    if (!outFile.is_open()) {
        cout << "Failed to open wallet file for writing: " << walletName + ".txt" << endl;
        return;
    }

    for (size_t i = 0; i < lines.size(); i++) {
        size_t cardPos = lines[i].find(cardNum);
        if (cardPos != string::npos) { 
            size_t balancePos = lines[i].find("Balance");
            if (balancePos != string::npos) {
                size_t colonPos = lines[i].find(":", balancePos);
                if (colonPos != string::npos) {
                    string balanceStr = lines[i].substr(colonPos + 1);
                    double balance = stod(balanceStr);
                    balance = newAmount;
                    lines[i] = lines[i].substr(0, colonPos + 1) + to_string(balance);
                }
            }
        }
        outFile << lines[i] << endl;
    }
    outFile.close();
}
void FileManager::FillInfoFromFile(string walletName, Wallet& wallet) {
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

                        card.getTransactions()[category].push_back(Transaction(category, amount, date));
                    }
                    break;
                }
            }

            wallet.addCard(card);
        }
    }

    file.close();
}

// Card
Card::Card(string num, string name, double bal) : number(num), holderName(name), balance(bal) {}

string Card::getNumber() {
    return number;
}

string Card::getName() {
    return holderName;
}

double Card::getBalance() {
    return balance;
}

void Card::setBalance(double amount) {
    balance = amount;
}

map<string, vector<Transaction>> Card::getTransactions() {
    return transactions;
}

void Card::withdraw(string category, double amount, string transactionDate) {
    if (balance >= amount) {
        balance = balance - amount;
        transactions[category].push_back(Transaction(category, amount, transactionDate));
    }
    else {
        cout << "Insufficient funds!" << endl;
    }
}

void Card::deposit(string walletName, double amount) {
    balance = balance + amount;
    //fileManager.updateBalance(walletName, balance);
}

void Card::printMenu() {
    cout << "Actions: " << endl;
    cout << "1 - Card top-up" << endl;
    cout << "2 - Make a transaction" << endl;
    cout << "0 - Exit " << endl;
}

void Card::printInfo() {
    cout << "Card number: " << number << endl;
    cout << "Holder's name: " << holderName << endl;
    cout << "Balance: " << balance << endl;
    for (auto& pair : transactions)
    {
        cout << "Category: " << pair.first << endl;
        for (Transaction& transaction : pair.second)
        {
            cout << " -" << transaction.getAmount() << "$" << endl;
            cout << transaction.getDate() << "\n\n";
        }
    }
}


// Wallet
Wallet::Wallet(string name, vector<Card> cardss) : holderName(name), cards(cardss) {}

string Wallet::getName() {
    return holderName;
}

vector<Card>& Wallet::getCards() {
    return cards;
}

void Wallet::setName(string name) {
    holderName = name;
}

void Wallet::setCards(vector<Card> cards) {
    cards = cards;
}

Card addCardPrint() {
    bool exitLoop = true;
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
    return card;
}

void Wallet::addCard(Card& card) {
    cards.push_back(card);
    FileManager::writeWalletFile(holderName, *this);
}

void Wallet::printMenu() {
    cout << "Menu: " << endl;
    cout << "1 - Add card" << endl;
    cout << "2 - Remove card" << endl;
    cout << "3 - Pick a card" << endl;
    cout << "0 - Exit " << endl;
}

void Wallet::printCards() {
    if (cards.empty())
    {
        cout << "There are no cards in this wallet" << endl;
    }
    else
    {
        //cout << "Your cards:" << endl;
        for (size_t i = 0; i < cards.size(); i++)
        {
            cout << i + 1 << " - " << cards[i].getNumber() << endl;
            cout << "    " << cards[i].getName() << "\n\n";
        }
    }  
}

void Wallet::printCardInfo(int index) {
    if (cards.empty())
    {
        cout << "There are no cards in this wallet" << endl;
        return;
    }
    else
    {
        cards[index-1].printInfo();
    }
}

void Wallet::printInfo(string walletname) {
    cout << walletname << endl;
    for (size_t i = 0; i < cards.size(); i++)
    {
        cout << "Number: " << cards[i].getNumber() << endl;
        cout << "Owner name: " << cards[i].getName() << endl;
        cout << "Balance: " << cards[i].getBalance() << endl;
        cout << endl;
    }

}

Card Wallet::getCardByIndex(int index) {
    return cards[index - 1];
}

// Static member initialization
FileManager Card::fileManager;
FileManager Wallet::fileManager;


int main() {
    Date date;
    string strDate = date.formatDateTime();
    string walletName;
    FileManager fm;
    Wallet wallet;
    Card card;
    cout << "Enter the name of your wallet: ";
    getline(cin, walletName);
    vector<Card> cards;
    if (!FileManager::fileExists(walletName + ".txt")) {
        cout << "No such wallet found" << endl;
        cout << "Creating a new wallet.." << endl;
        cout << "Enter wallet holder name -> ";
        string wholderName;
        getline(cin, wholderName);
        wallet.setName(wholderName);
        wallet.setCards(cards);
        cout << "name: " << walletName << endl;
        
        char ans;
        int counter = 0;
        cout << "Add card? y/n -> ";
        cin >> ans;
        cin.ignore();
        if (ans == 'y')
        {
            card = addCardPrint();
            wallet.getCards().push_back(card);

        }
        FileManager::writeWalletFile(walletName, wallet);
    }
    else {
        
        fm.FillInfoFromFile(walletName, wallet);  
    }
    outer_loop:
    while (true)
    {
        int choice;
        int cardchoice = 0;
        int cardmenu = 0;
        int amount = 0;
        string category;

        Sleep(1000);
        system("cls");
        wallet.printInfo(walletName);
        wallet.printMenu();
        cout << "-> ";
        cin >> choice;
        switch (choice) {
        case 1:
            Sleep(1500);
            system("cls");
            card = addCardPrint();
            wallet.getCards().push_back(card);
            cout << "Card added successfully!" << endl;
            system("pause");
            FileManager::writeWalletFile(walletName, wallet);
            break;
        case 2:
            cout << "You chose Option 2" << endl;
            break;
        case 3:
            Sleep(1500);
            system("cls");
            wallet.printCards();
            cout << "0 - Exit" << endl;
            cout << "-> ";
            cin >> cardchoice; 
            if (cardchoice == 0)
            {
                goto outer_loop;
            }
            Sleep(1500);
            system("cls");
            //wallet.getCardByIndex(cardchoice).printInfo();
            while (true)
            {
                wallet.getCardByIndex(cardchoice).printInfo();
                wallet.getCardByIndex(cardchoice).printMenu();
                cout << "-> ";
                cin >> cardmenu;
                switch (cardmenu)
                {
                case 1:
                    cout << "Enter amount -> ";
                    cin >> amount;
                    wallet.getCardByIndex(cardchoice).deposit(walletName, amount);
                    FileManager::writeWalletFile(walletName, wallet);
                    //wallet.getCardByIndex(cardchoice).setBalance(wallet.getCardByIndex(cardchoice).getBalance() + amount);
                    cout << "Successfull operation!" << endl;
                    //cout << "New balance: " << wallet.getCardByIndex(cardchoice).getBalance() << endl;
                    break;
                case 2:
                    cout << "Enter amount -> ";
                    cin >> amount;
                    cin.ignore();
                    cout << "Enter category - >";
                    getline(cin, category);
                    wallet.getCardByIndex(cardchoice).withdraw(category, amount, strDate);
                    FileManager::writeWalletFile(walletName, wallet);
                    cout << "Successfull operation!" << endl;
                    break;
                case 0:
                    goto outer_loop;
                    system("cls");
                    break;
                default:
                    cout << "Invalid choice!" << endl;
                    break;
                }
                system("pause");
                system("cls");
            }
            system("cls"); 
            break;
        case 0:
            exit(0);
            break;
        default:
            cout << "Invalid choice!" << endl;
            break;
        }
    }
   
    FileManager::writeWalletFile(walletName, wallet);
  
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