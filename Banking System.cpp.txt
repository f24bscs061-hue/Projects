//Including libraries
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

// Function prototypes
bool isAccountNumberUnique(const string& accountNumber);
void createAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
void transactionHistory();
void displayMenu();
bool findAccount(const string& accountNumber, string& name, double& balance);
void updateAccount(const string& accountNumber, const string& name, double balance);
void logTransaction(const string& accountNumber, const string& transaction);

// Global file names
const char accountFile[] = "accounts.txt";
const char historyFile[] = "transactions.txt";

int main() {
    int choice;

    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            createAccount();
            break;
        case 2:
            depositMoney();
            break;
        case 3:
            withdrawMoney();
            break;
        case 4:
            checkBalance();
            break;
        case 5:
            transactionHistory();
            break;
        case 6:
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}

void displayMenu() {
    cout << "\n======= Banking System Simulator =======\n";
    cout << "1. Create Account\n";
    cout << "2. Deposit Money\n";
    cout << "3. Withdraw Money\n";
    cout << "4. Check Balance\n";
    cout << "5. View Transaction History\n";
    cout << "6. Exit\n";
    cout << "=======================================\n";
}
bool isAccountNumberUnique(const string& accountNumber) {
    ifstream inFile(accountFile);
    if (!inFile) return true; // If the file doesn't exist, all account numbers are unique

    string accNumber, accName;
    double accBalance;

    while (inFile >> accNumber >> ws) { // Skip whitespaces
        getline(inFile, accName, ' '); // Read until the next space to capture the name
        inFile >> accBalance;
        if (accNumber == accountNumber) {
            inFile.close();

            return false; // Account number found, not unique

        }
    }
    inFile.close();
    return true;
}

void createAccount() {
    ofstream outFile(accountFile, ios::app);
    if (!outFile) {
        cout << "Error: Unable to open accounts file.\n";
        return;
    }

    string name, accountNumber;
    double initialDeposit;
    do {

        cout << "Enter a unique account number: ";
        cin >> accountNumber;

        if (!isAccountNumberUnique(accountNumber)) {
            cout << "This account number is already taken. Please try again.\n";
        }
    } while (!isAccountNumberUnique(accountNumber));

    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter initial deposit amount: ";
    cin >> initialDeposit;

    // Write account details to file
    outFile << accountNumber << " " << name << " " << fixed << setprecision(2) << initialDeposit << "\n";
    outFile.close();

    cout << "Account created successfully!\n";
}

bool findAccount(const string& accountNumber, string& name, double& balance) {
    ifstream inFile(accountFile);
    if (!inFile) {
        cout << "Error: Unable to open accounts file.\n";
        return false;
    }

    string accNumber, accName;
    double accBalance;
    while (inFile >> accNumber >> ws) {
        getline(inFile, accName, ' '); // Read until the next space to capture the name
        inFile >> accBalance;

        if (accNumber == accountNumber) {
            name = accName;
            balance = accBalance;
            inFile.close();
            return true;
        }
    }

    inFile.close();
    return false;
}

void updateAccount(const string& accountNumber, const string& name, double balance) {
    ifstream inFile(accountFile);
    ofstream tempFile("temp.txt");

    if (!inFile || !tempFile) {
        cout << "Error: Unable to open file for update.\n";
        return;
    }

    string accNumber, accName;
    double accBalance;
    bool accountUpdated = false;


    while (inFile >> accNumber >> ws) {
        getline(inFile, accName, ' '); // Read until the next space to capture the name
        inFile >> accBalance;

        if (accNumber == accountNumber) {
            tempFile << accNumber << " " << name << " " << fixed << setprecision(2) << balance << "\n";
            accountUpdated = true;
        }
        else {
            tempFile << accNumber << " " << accName << " " << fixed << setprecision(2) << accBalance << "\n";
        }
    }

    inFile.close();
    tempFile.close();

    // Replace original file with updated file
    remove(accountFile);
    rename("temp.txt", accountFile);

    if (!accountUpdated) {
        cout << "Error: Account not found.\n";
    }
}

void logTransaction(const string& accountNumber, const string& transaction) {
    ofstream historyOut(historyFile, ios::app);
    if (!historyOut) {
        cout << "Error: Unable to open transaction history file.\n";
        return;
    }

    historyOut << accountNumber << " " << transaction << "\n";
    historyOut.close();
}

void depositMoney() {
    string accountNumber, name;
    double balance, deposit;

    cout << "Enter your account number: ";
    cin >> accountNumber;

    if (!findAccount(accountNumber, name, balance)) {
        cout << "Error: Account not found.\n";
        return;
    }

    cout << "Enter amount to deposit: ";
    cin >> deposit;

    if (deposit <= 0) {
        cout << "Error: Deposit amount must be positive.\n";
        return;
    }

    balance += deposit;
    updateAccount(accountNumber, name, balance);
    logTransaction(accountNumber, "Deposited: " + to_string(deposit));

    cout << "Deposit successful!\n";
}

void withdrawMoney() {
    string accountNumber, name;
    double balance, withdrawal;

    cout << "Enter your account number: ";
    cin >> accountNumber;

    if (!findAccount(accountNumber, name, balance)) {
        cout << "Error: Account not found.\n";
        return;
    }

    cout << "Enter amount to withdraw: ";
    cin >> withdrawal;

    if (withdrawal <= 0) {
        cout << "Error: Withdrawal amount must be positive.\n";
        return;
    }

    if (withdrawal > balance) {
        cout << "Error: Insufficient funds.\n";
        return;
    }

    balance -= withdrawal;
    updateAccount(accountNumber, name, balance);
    logTransaction(accountNumber, "Withdrew: " + to_string(withdrawal));

    cout << "Withdrawal successful!\n";
}

void checkBalance() {
    string accountNumber, name;
    double balance;

    cout << "Enter your account number: ";
    cin >> accountNumber;

    if (!findAccount(accountNumber, name, balance)) {
        cout << "Error: Account not found.\n";
        return;
    }

    cout << "\nAccount Holder: " << name << "\n";
    cout << "Current Balance: " << balance << "\n";
}

void transactionHistory() {
    ifstream historyIn(historyFile);
    if (!historyIn) {
        cout << "Error: No transaction history found.\n";
        return;
    }

    string accountNumber, line;
    cout << "Enter your account number: ";
    cin >> accountNumber;

    cout << "\nTransaction History for Account " << accountNumber << ":\n";
    bool found = false;
    while (getline(historyIn, line)) {
        if (line.find(accountNumber) == 0) {
            cout << line.substr(accountNumber.length() + 1) << "\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No transactions found for this account.\n";
    }

    historyIn.close();
}
