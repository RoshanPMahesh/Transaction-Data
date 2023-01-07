#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

// Struct for all Transaction data, which includes CustomerID, date, and the amount of the transaction
struct Transaction {
    int customerId;
    string date;
    double amount;
};

// Struct for all Balance data, which includes the minimum, maximum, and ending balance
// Struct also includes the daily balance, which is used so that the min, max, and ending balance calculations are easier
struct Balance {
    double min = INT16_MAX;
    double max = INT16_MIN;
    double ending;
    double daily;
};
