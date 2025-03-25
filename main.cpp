#include <iostream>
#include <memory>
#include <stdexcept>

using namespace std;

// TODO: Define custom exception classes here
// - NegativeDepositException (for negative deposit attempts)
class NegativeDepositException : public exception {
    private: 
    string Errormessage;

    public: 
    NegativeDepositException (double amount) {
            Errormessage = "Deposit amount " +to_string(amount) + " cannot be negative, please enter valid amount:  \n"; 
            }
            const char* what() const noexcept override {
                return Errormessage.c_str();
            }


        
};


// - OverdrawException (for withdrawing more than the balance)
class OverdrawException : public exception {
    private: 
    string Errormessage;

    public: 
    OverdrawException (double amount) {
            Errormessage = "Invalid amount: "  + to_string(amount) + " cannot withdraw more than balance:  \n"; 
            }
            const char* what() const noexcept override {
                return Errormessage.c_str();
            }

};

// - InvalidAccountOperationException (for transactions on a closed account)

class AccountOperationException : public exception {
    private: 
    string Errormessage;

    public: 
    AccountOperationException (string accountNumber) {
            Errormessage = "Invalid transaction, account number: " + accountNumber + " is closed:  \n"; 
            }
            const char* what() const noexcept override{
                return Errormessage.c_str();
            }

};


    

class BankAccount {
private:
    std::string accountNumber;
    double balance;
    bool isActive;

public:
    // Constructor to initialize account number, balance, and active status
    BankAccount(string accNum, double initialBalance) : accountNumber(accNum), balance(initialBalance), isActive(true) {}

    // Deposit method
    void deposit(double amount) {
        // Implemented exception handling for negative deposits
        
        if (!isActive) {
            throw AccountOperationException(accountNumber);
        }
         else if (amount < 0) {
                throw NegativeDepositException(amount);
            }

        balance += amount;
    
    }

    // Withdraw method
    void withdraw(double amount) {
        // TODO: Implement exception handling for overdrawing and closed accounts
       
        if (!isActive) {
            throw AccountOperationException(accountNumber);
        }
         else if (balance < amount) {
                throw OverdrawException(amount);
            }
        balance -= amount;

    }
       

    // Method to get current balance
    double getBalance()  {
        return balance;
    }

    // Method to close the account
    void closeAccount() {
        // TODO: Prevent further transactions when the account is closed
        isActive = false;
    }
// method to get the active status 
    bool getActiveStatus() {
        return isActive;
    }
// method to get the account number
    string getAccountNumber() {
        return accountNumber;
    }
};

int main() {
    int choice;
    double amount, initialBalance;

// input validation for the initial balance
    while (true) {
        cout << "Welcome to the Bank. Please Enter your initial balance: ";
        cin >> initialBalance;
    
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n'); 
            cout << "Invalid input. Please enter a valid number.\n";
            continue;
        }
    
        try {
            if (initialBalance < 0) {
                throw NegativeDepositException(initialBalance);
            }
            break; // valid input
        } catch (const NegativeDepositException& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    try {
// creating a unique pointer to the bank account
        auto account = make_unique<BankAccount>("123456", initialBalance);
        cout << "Bank Account Created: #" << account->getAccountNumber() << endl;
// menu for the user to choose the transaction
        do {
            cout << "\n--- Menu ---\n";
            cout << "1. Deposit\n";
            cout << "2. Withdraw\n";
            cout << "3. Close Account\n";
            cout << "4. Exit\n";
            cout << "Enter choice (1-4): ";
            cin >> choice;

            // Validation for the menu choice
            while (choice < 1 || choice > 4) {
                cout << "Invalid choice. Please enter 1 to 4: ";
                cin >> choice;
            }
// switch case for the menu choice
            try {
                switch (choice) {
                    case 1:
                        cout << "Enter deposit amount: ";
                        cin >> amount;
                        account->deposit(amount);
                        cout << "Deposit successful. New Balance is: $" << account->getBalance() << endl;
                        break;

                    case 2:
                        cout << "Enter withdrawl amount: ";
                        cin >> amount;
                        account->withdraw(amount);
                        cout << "Withdrawal successful. New Balance is:: $" << account->getBalance() << endl;
                        break;

                    case 3:
                        account->closeAccount();
                        cout << "Account closed successfully.\n";
                        break;

                    case 4:
                        cout << "Exiting program.\n";
                        break;
                }
                // Exception handling for the custom exceptions
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }

        } while (choice != 4);
// checking if the account is closed or not
    } catch (const exception& e) {
        cerr << "Error creating account: " << e.what() << endl;
    }

    return 0;
}
