#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include "Customer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <sstream>
using namespace std;

class OrderManager
{
private:
    static const string ORDER_FILE;
    static const string TEMP_FILE;

public:
    static void saveOrder(const Customer &c)
    {
        ofstream file(ORDER_FILE.c_str(), ios::app);
        if (!file) { cerr << "Error opening order file!\n"; return; }
        file << "Name: "    << c.getName()        << "\n";
        file << "Email: "   << c.getEmail()       << "\n";
        file << "Phone: "   << c.getPhoneNumber() << "\n";
        file << "OrderID: " << c.getOrderID()     << "\n";
        file.close();
    }

    static void searchOrder(int searchID)
    {
        ifstream file(ORDER_FILE.c_str());
        if (!file) { cout << "No orders found (file missing).\n"; return; }

        string line, block = "";
        ostringstream oss; oss << searchID; string searchStr = oss.str();
        bool found = false;

        while (getline(file, line))
        {
            if (line.find("Name:") == 0) block = "";
            block += line + "\n";
            if (line.find("OrderID: " + searchStr) != string::npos)
            {
                cout << "============================================================\n";
                cout << "                    ORDER FOUND\n";
                cout << "============================================================\n";
                cout << block;
                found = true;
                break;
            }
        }
        if (!found) cout << "Order with ID " << searchID << " not found.\n";
        file.close();
    }

    static void displayAllOrders()
    {
        ifstream file(ORDER_FILE.c_str());
        if (!file) { cout << "No saved orders found.\n"; return; }
        cout << "============================================================\n";
        cout << "                    ALL SAVED ORDERS\n";
        cout << "============================================================\n";
        string line;
        while (getline(file, line)) cout << line << "\n";
        cout << "============================================================\n";
        file.close();
    }

    static bool cancelOrder(int cancelID)
    {
        ifstream file(ORDER_FILE.c_str());
        ofstream temp(TEMP_FILE.c_str());
        if (!file || !temp) { cerr << "File error during cancellation.\n"; return false; }

        ostringstream oss2; oss2 << cancelID; string line, cancelStr = oss2.str();
        bool found = false, skip = false;

        while (getline(file, line))
        {
            if (line.find("Name:") == 0)                               skip = false;
            if (line.find("OrderID: " + cancelStr) != string::npos) { found = true; skip = true; }
            if (!skip) temp << line << "\n";
        }
        file.close(); temp.close();

        if (found)
        {
            remove(ORDER_FILE.c_str());
            rename(TEMP_FILE.c_str(), ORDER_FILE.c_str());
            cout << "Order " << cancelID << " cancelled successfully!\n";
        }
        else
        {
            remove(TEMP_FILE.c_str());
            cout << "Order ID " << cancelID << " not found.\n";
        }
        return found;
    }
};

const string OrderManager::ORDER_FILE = "OrderFile.txt";
const string OrderManager::TEMP_FILE  = "temp.txt";

// ============================================================
//  Customer::placeOrder  — full screen-flow with clearScreen
// ============================================================
void Customer::placeOrder()
{
    Menu menu;

    // ── STEP 1: Ask if they want to see the menu ─────────────
    clearScreen();
    cout << "============================================================\n";
    cout << "                   RESTAURANT ORDER\n";
    cout << "============================================================\n";
    cout << "\n  Would you like to view our menu before ordering?\n\n";
    cout << "  1. Yes, show me the menu\n";
    cout << "  2. No, I already know what I want\n";
    cout << "------------------------------------------------------------\n";
    cout << "  Enter choice: ";
    int viewChoice;
    cin >> viewChoice;

    if (viewChoice == 1)
    {
        // ── STEP 2: Clear screen and show MENU ONLY ──────────
        clearScreen();
        menu.displayMenu();
        cout << "\n";

        while (true)
        {
            cout << "  Are you ready to place your order?\n";
            cout << "  1. Yes, go to cart\n";
            cout << "  2. No, let me read the menu again\n";
            cout << "  Enter choice: ";
            int readyChoice;
            cin >> readyChoice;
            if (readyChoice == 1) break;
            clearScreen();
            menu.displayMenu();
            cout << "\n";
        }
    }

    // ── STEP 3: Clear screen → ADD TO CART ───────────────────
    clearScreen();
    cout << "============================================================\n";
    cout << "                       ADD TO CART\n";
    cout << "============================================================\n";
    cout << "  MENU REFERENCE:\n";
    cout << "  ------------------------------------------------------------\n";
    for (int i = 0; i < menu.getTotalItems(); i++)
    {
        cout << "  " << (i + 1) << ". "
             << menu.getNameByNumber(i + 1)
             << "  ---  " << menu.getPriceByNumber(i + 1) << " RS\n";
    }
    cout << "  ------------------------------------------------------------\n";
    cout << "  Enter item number to add  |  0 = Checkout  |  M = Full menu\n";
    cout << "============================================================\n\n";

    string input;
    while (true)
    {
        if (itemCount > 0)
            cout << "  Cart (" << itemCount << " item"
                 << (itemCount > 1 ? "s" : "") << ")  |  Total: "
                 << recursiveBillSum() << " RS\n";

        cout << "  >> Add to cart (number / 0 to checkout / M for menu): ";
        cin >> input;

        // M = view full menu then return to cart
        if (input == "M" || input == "m")
        {
            clearScreen();
            menu.displayMenu();
            pauseScreen();
            clearScreen();
            cout << "============================================================\n";
            cout << "                       ADD TO CART\n";
            cout << "============================================================\n";
            cout << "  MENU REFERENCE:\n";
            cout << "  ------------------------------------------------------------\n";
            for (int i = 0; i < menu.getTotalItems(); i++)
            {
                cout << "  " << (i + 1) << ". "
                     << menu.getNameByNumber(i + 1)
                     << "  ---  " << menu.getPriceByNumber(i + 1) << " RS\n";
            }
            cout << "  ------------------------------------------------------------\n";
            cout << "  Enter item number to add  |  0 = Checkout  |  M = Full menu\n";
            cout << "============================================================\n\n";
            continue;
        }

        // Parse number
        int choice = 0;
        bool isNum = true;
        for (int i = 0; i < (int)input.size(); i++)
            if (input[i] < '0' || input[i] > '9') { isNum = false; break; }
        if (isNum && !input.empty()) choice = atoi(input.c_str());

        if (choice == 0)
        {
            if (itemCount == 0) { cout << "  [!] Cart is empty — add at least one item.\n\n"; continue; }
            break;
        }

        int price = menu.getPriceByNumber(choice);
        if (price == -1)
        {
            cout << "  [!] Invalid number. Choose 1-" << menu.getTotalItems() << ".\n\n";
            continue;
        }
        addItem(menu.getNameByNumber(choice), price);
        cout << "  [+] Added: " << menu.getNameByNumber(choice)
             << "  (" << price << " RS)\n\n";
    }

    // ── STEP 4: Confirm cart ──────────────────────────────────
    clearScreen();
    cout << "============================================================\n";
    cout << "                    CART CONFIRMED\n";
    cout << "============================================================\n";
    for (int i = 0; i < itemCount; i++)
        cout << "  " << (i + 1) << ". " << orderedItems[i]
             << "  ---  " << orderedPrices[i] << " RS\n";
    cout << "------------------------------------------------------------\n";
    cout << "  Items   : " << itemCount << "\n";
    cout << "  Subtotal: " << recursiveBillSum() << " RS\n";
    cout << "============================================================\n";

    saveToFile();
    cout << "\n  Order placed!  Your Order ID: " << orderID << "\n";
    pauseScreen();
}

void Customer::cancelOrder()
{
    cout << "Enter Order ID to cancel: ";
    int id; cin >> id;
    OrderManager::cancelOrder(id);
}

void Customer::changeOrder()
{
    cout << "Changing order clears your current items.\n";
    itemCount = 0;
    placeOrder();
}

#endif
