#include "balance.h"

int main() {
    // Read transactions from CSV file called transactions.csv
    ifstream file("transactions.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening input file" << std::endl; //outputs an error if the file couldn't be opened and read from
        return 1;
    }

    // All the data types that are used for this problem
    string line; // Used to read each line from CSV file
    map<int, map<string, Balance>> balances; // Used to keep the CustomerID, date, min, max, and ending balance that will be printed
    map<int, map<string, Balance>> positiveNum; // Keeps track of all positive transactions for use in cases of multiple transactions in a day
    map<int, map<string, Balance>> negativeNum; // Keeps track of all negative transactions for use in cases of multiple transactions in a day
    map<int, map<string, Balance>> keepTrack; // Keeps track of all the individual dates so that we can iterate through each day to do calculations
    char firstLetter; // Keeps track of the first letter in the CustomerID

    // Parses through each line of the CSV file data
    while (getline(file, line)) {
        if (line == "" || line[1] == ',') {
            continue; // Skips past blank spaces - not sure if blank spaces are denoted by a literal blank or by commas, so I checked for both
        } else {
            stringstream ss(line);
            string cell;
            Transaction transaction;
            getline(ss, cell, ',');
            firstLetter = cell[0];
            cell.erase(0, 1);
            transaction.customerId = stoi(cell); //Stores the CustomerID
            getline(ss, cell, ',');
            transaction.date = cell; // Stores the date
            getline(ss, cell, ',');
            transaction.amount = stod(cell); // Stores the transaction amount

            // Apply transaction amount to negativeNum if the amount is negative, or to positiveNum if the amount is positive
            if (transaction.amount < 0) {
                negativeNum[transaction.customerId][transaction.date].daily += transaction.amount;
            } else {
                positiveNum[transaction.customerId][transaction.date].daily += transaction.amount;
            }

            keepTrack[transaction.customerId][transaction.date].daily = 0;
        }
    }

    // Iterates through the keepTrack map, which stored each individual date (all 30 days in each month)
    for (auto check1 : keepTrack) {
        for (auto check2 : check1.second) {

            // Takes just the month and year from the date
            string month = check2.first.substr(0, 3);
            string year = check2.first.substr(6,10);
            string monthYear = month + year;

            // Checks to see if there are credits and debits on the same day, then applies credit transactions first
            if (negativeNum[check1.first].find(check2.first) != negativeNum[check1.first].end() && positiveNum[check1.first].find(check2.first) != positiveNum[check1.first].end()) {
                balances[check1.first][monthYear].ending += positiveNum[check1.first][check2.first].daily;
                balances[check1.first][monthYear].min = min(balances[check1.first][monthYear].min, balances[check1.first][monthYear].ending);
                balances[check1.first][monthYear].max = max(balances[check1.first][monthYear].max, balances[check1.first][monthYear].ending);

                balances[check1.first][monthYear].ending += negativeNum[check1.first][check2.first].daily;
                balances[check1.first][monthYear].min = min(balances[check1.first][monthYear].min, balances[check1.first][monthYear].ending);
                balances[check1.first][monthYear].max = max(balances[check1.first][monthYear].max, balances[check1.first][monthYear].ending);

            // Checks to see if there are only debits on a given day, then applies the debit transaction
            } else if (negativeNum[check1.first].find(check2.first) != negativeNum[check1.first].end()) {
                balances[check1.first][monthYear].ending += negativeNum[check1.first][check2.first].daily;
                balances[check1.first][monthYear].min = min(balances[check1.first][monthYear].min, balances[check1.first][monthYear].ending);
                balances[check1.first][monthYear].max = max(balances[check1.first][monthYear].max, balances[check1.first][monthYear].ending);

            // Checks to see if there are only credits on a given day, then applies the credit transaction
            } else {
                balances[check1.first][monthYear].ending += positiveNum[check1.first][check2.first].daily;
                balances[check1.first][monthYear].min = min(balances[check1.first][monthYear].min, balances[check1.first][monthYear].ending);
                balances[check1.first][monthYear].max = max(balances[check1.first][monthYear].max, balances[check1.first][monthYear].ending);
            }
        }
    }

    // Place the output in a .csv file called insights.csv
    ofstream outFile("insights.csv");
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file" << std::endl; // Outputs an error if the file couldn't be opened
        return 1;
    }

    // Iterates through the balances map to store CustomerID, date, minimum, maximum, and ending balances in insights.csv file
    // Also outputs the same data in the balances map to the terminal
    for (auto customerBalances : balances) {
        for (auto monthBalance : customerBalances.second) {
            outFile << firstLetter << customerBalances.first << ", " << monthBalance.first << ", " << monthBalance.second.min << ", " << monthBalance.second.max << ", " << monthBalance.second.ending << endl;
            cout << firstLetter << customerBalances.first << ", " << monthBalance.first << ", " << monthBalance.second.min << ", " << monthBalance.second.max << ", " << monthBalance.second.ending << endl;
        }
    }

    return 0;
}
