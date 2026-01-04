#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class Account {
    int accountNumber;
    string holderName;
    double balance;

public:
    Account(int accNo, string name, double bal)
        : accountNumber(accNo), holderName(name), balance(bal) {}

    int getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (amount > balance) return false;
        balance -= amount;
        return true;
    }

    void display() const {
        cout << "\nAccount Number : " << accountNumber;
        cout << "\nHolder Name    : " << holderName;
        cout << "\nBalance        : " << fixed << setprecision(2) << balance << endl;
    }

    void save(ofstream &out) const {
        out << accountNumber << " " << holderName << " " << balance << endl;
    }
};

vector<Account> loadAccounts() {
    vector<Account> accounts;
    ifstream in("accounts.txt");

    int accNo;
    string name;
    double bal;

    while (in >> accNo >> name >> bal) {
        accounts.emplace_back(accNo, name, bal);
    }
    in.close();
    return accounts;
}

void saveAccounts(const vector<Account> &accounts) {
    ofstream out("accounts.txt");
    for (const auto &acc : accounts) {
        acc.save(out);
    }
    out.close();
}

Account* findAccount(vector<Account> &accounts, int accNo) {
    for (auto &acc : accounts) {
        if (acc.getAccountNumber() == accNo)
            return &acc;
    }
    return nullptr;
}

void createAccount(vector<Account> &accounts) {
    int accNo;
    string name;
    double balance;

    cout << "Enter account number: ";
    cin >> accNo;

    if (findAccount(accounts, accNo)) {
        cout << "Account already exists.\n";
        return;
    }

    cout << "Enter holder name: ";
    cin >> name;

    cout << "Enter initial balance: ";
    cin >> balance;

    accounts.emplace_back(accNo, name, balance);
    saveAccounts(accounts);

    cout << "Account created successfully.\n";
}

void depositMoney(vector<Account> &accounts) {
    int accNo;
    double amount;

    cout << "Enter account number: ";
    cin >> accNo;

    Account *acc = findAccount(accounts, accNo);
    if (!acc) {
        cout << "Account not found.\n";
        return;
    }

    cout << "Enter amount to deposit: ";
    cin >> amount;

    acc->deposit(amount);
    saveAccounts(accounts);

    cout << "Deposit successful.\n";
}

void withdrawMoney(vector<Account> &accounts) {
    int accNo;
    double amount;

    cout << "Enter account number: ";
    cin >> accNo;

    Account *acc = findAccount(accounts, accNo);
    if (!acc) {
        cout << "Account not found.\n";
        return;
    }

    cout << "Enter amount to withdraw: ";
    cin >> amount;

    if (!acc->withdraw(amount)) {
        cout << "Insufficient balance.\n";
        return;
    }

    saveAccounts(accounts);
    cout << "Withdrawal successful.\n";
}

void viewAccount(const vector<Account> &accounts) {
    int accNo;
    cout << "Enter account number: ";
    cin >> accNo;

    for (const auto &acc : accounts) {
        if (acc.getAccountNumber() == accNo) {
            acc.display();
            return;
        }
    }
    cout << "Account not found.\n";
}

void showMenu() {
    cout << "\n--- Bank Management System ---\n";
    cout << "1. Create Account\n";
    cout << "2. Deposit Money\n";
    cout << "3. Withdraw Money\n";
    cout << "4. View Account Details\n";
    cout << "5. Exit\n";
    cout << "Enter choice: ";
}

int main() {
    vector<Account> accounts = loadAccounts();
    int choice;

    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1: createAccount(accounts); break;
            case 2: depositMoney(accounts); break;
            case 3: withdrawMoney(accounts); break;
            case 4: viewAccount(accounts); break;
            case 5: cout << "Exiting system.\n"; break;
            default: cout << "Invalid choice.\n";
        }

    } while (choice != 5);

    return 0;
}