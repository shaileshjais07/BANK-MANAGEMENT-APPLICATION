#include <iostream>
#include <fstream>
using namespace std;

class BankAccount {
private:
    int accNo;
    char name[50];
    float balance;
    int pin;

public:
    void createAccount() {
        cout << "Enter Account Number: ";
        cin >> accNo;

        cout << "Enter Name: ";
        cin.ignore();
        cin.getline(name, 50);

        cout << "Enter Initial Balance: ";
        cin >> balance;

        cout << "Set 4-digit PIN: ";
        cin >> pin;
    }

    void showAccount() {
        cout << "\nAccount No: " << accNo;
        cout << "\nName: " << name;
        cout << "\nBalance: " << balance << endl;
    }

    int getAccNo() {
        return accNo;
    }

    int getPin() {
        return pin;
    }

    void deposit(float amt) {
        balance += amt;
    }

    bool withdraw(float amt) {
    if (amt > balance) {
        return false;   // ❌ fail
    } else {
        balance -= amt;
        return true;    // ✅ success
    }
}
};

// Create Account
void writeAccount() {
    BankAccount acc;
    ofstream outFile("bank.dat", ios::binary | ios::app);

    acc.createAccount();
    outFile.write((char*)&acc, sizeof(acc));

    outFile.close();
    cout << "Account Created Successfully!\n";
}

// Deposit / Withdraw
void transaction(int accNo, int pin, int option) {
    BankAccount acc;
    fstream file("bank.dat", ios::binary | ios::in | ios::out);

    bool found = false;

    while (file.read((char*)&acc, sizeof(acc))) {
        if (acc.getAccNo() == accNo && acc.getPin() == pin) {

            float amt;
            cout << "Enter Amount: ";
            cin >> amt;

            if (option == 1) {
                acc.deposit(amt);
                cout << "Deposit Successful!\n";
            }
            else {
                if (acc.withdraw(amt)) {
                    cout << "Withdrawal Successful!\n";
                } else {
                    cout << "Insufficient Balance!\n";
                    file.close();
                    return;   // 🔥 STOP here (no update)
                }
            }

            int pos = -sizeof(acc);
            file.seekp(pos, ios::cur);
            file.write((char*)&acc, sizeof(acc));

            found = true;
            break;
        }
    }

    if (!found)
        cout << "Invalid Account Number or PIN!\n";

    file.close();
}

// Balance Inquiry
void checkBalance(int accNo, int pin) {
    BankAccount acc;
    ifstream inFile("bank.dat", ios::binary);

    bool found = false;

    while (inFile.read((char*)&acc, sizeof(acc))) {
        if (acc.getAccNo() == accNo && acc.getPin() == pin) {
            cout << "\n===== ACCOUNT DETAILS =====\n";
            acc.showAccount();
            found = true;
            break;   
        }
    }

    if (!found)
        cout << "Invalid Account Number or PIN!\n";

    inFile.close();
}

// Main
int main() {
    int choice, accNo, pin;

    do {
        cout << "\n====== BANK MANAGEMENT SYSTEM ======\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Balance Inquiry\n";
        cout << "5. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            writeAccount();
            break;

        case 2:
            cout << "Enter Account Number: ";
            cin >> accNo;
            cout << "Enter PIN: ";
            cin >> pin;
            transaction(accNo, pin, 1);
            break;

        case 3:
            cout << "Enter Account Number: ";
            cin >> accNo;
            cout << "Enter PIN: ";
            cin >> pin;
            transaction(accNo, pin, 2);
            break;

        case 4:
            cout << "Enter Account Number: ";
            cin >> accNo;
            cout << "Enter PIN: ";
            cin >> pin;
            checkBalance(accNo, pin);
            break;

        case 5:
            cout << "Thank You!\n";
            break;

        default:
            cout << "Invalid Choice!\n";
        }

    } while (choice != 5);

    return 0;
}
