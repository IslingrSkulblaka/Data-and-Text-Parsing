#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

struct Expense {
    std::string date;
    std::string category;
    double amount;
};

// Reads CSV file
std::vector<Expense> loadExpenses(const std::string& filename) {
    std::vector<Expense> expenses;
    std::ifstream file(filename);

    if (!file.is_open()) {
        return expenses; // Returns empty list if file doesn't exist yet
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date, category, amountStr;

        if (std::getline(ss, date, ',') &&
            std::getline(ss, category, ',') &&
            std::getline(ss, amountStr, ',')) {

            Expense exp;
            exp.date = date;
            exp.category = category;
            try {
                exp.amount = std::stod(amountStr);
                expenses.push_back(exp);
            }
            catch (...) {
                continue; // Skip corrupted lines
            }
        }
    }
    file.close();
    return expenses;
}

// Appends to CSV file
void saveExpense(const std::string& filename, const Expense& exp) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << exp.date << "," << exp.category << "," << exp.amount << "\n";
        file.close();
    }
}

int main() {
    const std::string filename = "expenses.csv";
    int choice = 0;

    std::cout << "=== Personal Expense Tracker ===\n";

    while (true) {
        std::cout << "\nChoose an option:\n";
        std::cout << "1. View All Expenses\n";
        std::cout << "2. Add New Expense\n";
        std::cout << "3. Exit Program\n";
        std::cout << "Enter number (1-3): ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        // Clear the input buffer trailing newline
        std::cin.ignore(10000, '\n');

        if (choice == 1) {
            std::vector<Expense> expenses = loadExpenses(filename);
            if (expenses.empty()) {
                std::cout << "\nNo expenses found yet.\n";
            }
            else {
                double total = 0;
                std::cout << "\n--- Current Expenses ---\n";
                for (const auto& exp : expenses) {
                    std::cout << exp.date << " | " << exp.category << " | $" << exp.amount << "\n";
                    total += exp.amount;
                }
                std::cout << "---------------------\n";
                std::cout << "Total Spend: $" << total << "\n";
            }

        }
        else if (choice == 2) {
            Expense newExp;
            std::string amountInput;

            std::cout << "\nEnter Date (YYYY-MM-DD): ";
            std::getline(std::cin, newExp.date);

            std::cout << "Enter Category (e.g., Food, Rent): ";
            std::getline(std::cin, newExp.category);

            std::cout << "Enter Amount: $";
            std::getline(std::cin, amountInput);

            try {
                newExp.amount = std::stod(amountInput);
                saveExpense(filename, newExp);
                std::cout << "Expense successfully added!\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: Invalid amount format. Expense not saved.\n";
            }

        }
        else if (choice == 3) {
            std::cout << "Goodbye!\n";
            break;
        }
        else {
            std::cout << "Invalid selection. Please choose 1, 2, or 3.\n";
        }
    }

    return 0;
}