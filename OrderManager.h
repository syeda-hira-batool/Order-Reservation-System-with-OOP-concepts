#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include "Customer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>   // for remove(), rename()
using namespace std;

// ============================================================
//  CLASS: OrderManager
//  PERSON 2 - Demonstrates: File I/O (Filing), Encapsulation,
//             Static functions, Const functions
// ============================================================
class OrderManager {
private:
    static const string ORDER_FILE;
    static const string TEMP_FILE;

public:
    // ---------- FILE I/O (Filing - Week 13-14 topic) ----------

    // Save a customer order to file
    static void saveOrder(const Customer& c) {
        ofstream file(ORDER_FILE, ios::app);
        if (!file) {
            cerr << "Error opening order file!\n";
            return;
        }
        file << "Name: "    << c.getName()        << "\n";
        file << "Email: "   << c.getEmail()       << "\n";
        file << "Phone: "<< c.getPhoneNumber() << "\n";
        file << "OrderID: " << c.getOrderID()     << "\n";

        // We need access to items - use displayOrder logic via file
        // (Customer saves itself, so call Customer's saveToFile)
        file.close();
        // Actually delegate to customer's own save method
        // (called separately from main)
    }

    // Search order by ID from file
    static void searchOrder(int searchID) {
        ifstream file(ORDER_FILE);
        if (!file) {
            cout << "No orders found (file missing).\n";
            return;
        }

        string line, block = "";
        string searchStr = to_string(searchID);
        bool found = false;

        while (getline(file, line)) {
            // Start of a new customer block
            if (line.find("Name:") == 0) {
                block = "";
            }
            block += line + "\n";

            // Check if this block has matching OrderID
            if (line.find("OrderID: " + searchStr) != string::npos) {
                cout << "============================================================\n";
                cout << "                    ORDER FOUND\n";
                cout << "============================================================\n";
                cout << block;
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Order with ID " << searchID << " not found.\n";
        }
        file.close();
    }

    // Display all orders from file
    static void displayAllOrders() {
        ifstream file(ORDER_FILE);
        if (!file) {
            cout << "No saved orders found.\n";
            return;
        }
        cout << "============================================================\n";
        cout << "                    ALL SAVED ORDERS\n";
        cout << "============================================================\n";
        string line;
        while (getline(file, line)) {
            cout << line << "\n";
        }
        cout << "============================================================\n";
        file.close();
    }

    // Cancel order by ID (rewrites file excluding cancelled order)
    static bool cancelOrder(int cancelID) {
        ifstream file(ORDER_FILE);
        ofstream temp(TEMP_FILE);
        if (!file || !temp) {
            cerr << "File error during cancellation.\n";
            return false;
        }

        string line;
        string cancelStr = to_string(cancelID);
        bool   found     = false;
        bool   skip      = false;

        while (getline(file, line)) { 
            if (line.find("Name:") == 0) {
                skip = false;
            }

            if (line.find("OrderID: " + cancelStr) != string::npos) {
                found = true;
                skip  = true;
                
            }

            if (!skip) {
                temp << line << "\n";
            }
        }

        file.close();
        temp.close();

        if (found) {
            remove(ORDER_FILE.c_str());
            rename(TEMP_FILE.c_str(), ORDER_FILE.c_str());
            cout << "Order " << cancelID << " cancelled successfully!\n";
        } else {
            remove(TEMP_FILE.c_str());
            cout << "Order ID " << cancelID << " not found.\n";
        }
        return found;
    }
};
const string OrderManager::ORDER_FILE = "OrderFile.txt";
const string OrderManager::TEMP_FILE  = "temp.txt";
void Customer::placeOrder() {
    Menu menu;
    menu.displayMenu();

    int itemCount_input;
    cout << "How many items do you want to order? ";
    cin  >> itemCount_input;

    for (int i = 0; i < itemCount_input; i++) {
        int choice;
        cout << "Enter item number: ";
        cin  >> choice;

        int price = menu.getPriceByNumber(choice);
        if (price == -1) {
            cout << "Invalid item number! Skipping.\n";
            continue;
        }
        addItem(menu.getNameByNumber(choice), price);
        cout << "  Added: " << menu.getNameByNumber(choice)
             << " (" << price << " RS)\n";
    }

    saveToFile();
    cout << "Order placed successfully! Your Order ID: " << orderID << "\n";
}

void Customer::cancelOrder() {
    cout << "Enter Order ID to cancel: ";
    int id;
    cin  >> id;
    OrderManager::cancelOrder(id);
}

void Customer::changeOrder() {
    cout << "Changing order clears your current items.\n";
    itemCount = 0;
    cout << "Re-ordering...\n";
    placeOrder();
}

#endif
