#include "../include/FileManager.h"
#include <iostream>
#include <cstdio>
using namespace std;

const string FileManager::ORDER_FILE = "OrderFile.txt";
const string FileManager::TEMP_FILE = "temp.txt";

void FileManager::saveCustomer(const Customer &c)
{
    ofstream file(ORDER_FILE, ios::app);
    if (!file.is_open())
        throw runtime_error("Cannot open " + ORDER_FILE + " for writing.");

    file << "Name: " << c.getName() << "\n";
    file << "Email: " << c.getEmail() << "\n";
    file << "Phone: +92" << c.getPhone() << "\n";
    file << "OrderID: " << c.getOrder().getOrderID() << "\n";

    for (const auto &item : c.getOrder().getItems())
        file << "Item: " << item.getName() << "\n";

    if (c.isDelivery())
        file << "Delivery: " << c.getDeliveryAddress() << "\n";

    file << "Total: " << c.getFinalBill() << " RS\n";
    file << "----\n";
    file.close();
}

bool FileManager::searchOrder(int orderID)
{
    ifstream file(ORDER_FILE);
    if (!file.is_open())
    {
        cout << "No orders file found.\n";
        return false;
    }

    string line, block;
    bool found = false;
    bool inBlock = false;
    int currentID = -1;

    while (getline(file, line))
    {
        if (line.rfind("Name:", 0) == 0)
        {
            block = "";
            inBlock = true;
            currentID = -1;
        }
        if (inBlock)
            block += line + "\n";

        if (line.rfind("OrderID:", 0) == 0)
        {
            try
            {
                currentID = stoi(line.substr(9));
            }
            catch (...)
            {
            }
        }

        if (line == "----" && currentID == orderID)
        {
            cout << "============================================================\n";
            cout << " Order Found\n";
            cout << "============================================================\n";
            cout << block;
            found = true;
            break;
        }
    }

    file.close();
    if (!found)
        cout << "Order ID " << orderID << " not found.\n";
    return found;
}

bool FileManager::cancelOrder(int orderID)
{
    ifstream src(ORDER_FILE);
    ofstream tmp(TEMP_FILE);

    if (!src.is_open() || !tmp.is_open())
    {
        throw runtime_error("File error during cancellation.");
    }

    string line;
    bool found = false;

    vector<vector<string>> allBlocks;
    vector<string> current;

    while (getline(src, line))
    {
        current.push_back(line);
        if (line == "----")
        {
            allBlocks.push_back(current);
            current.clear();
        }
    }
    src.close();

    for (auto &blk : allBlocks)
    {
        bool isTarget = false;
        for (const auto &l : blk)
        {
            if (l.rfind("OrderID:", 0) == 0)
            {
                try
                {
                    if (stoi(l.substr(9)) == orderID)
                    {
                        isTarget = true;
                        found = true;
                    }
                }
                catch (...)
                {
                }
            }
        }
        if (!isTarget)
        {
            for (const auto &l : blk)
                tmp << l << "\n";
        }
    }

    tmp.close();
    remove(ORDER_FILE.c_str());
    rename(TEMP_FILE.c_str(), ORDER_FILE.c_str());

    if (found)
        cout << "Order " << orderID << " cancelled successfully.\n";
    else
        cout << "Order ID " << orderID << " not found.\n";

    return found;
}

void FileManager::displayLastOrder()
{
    ifstream file(ORDER_FILE);
    if (!file.is_open())
    {
        cout << "No orders on record yet.\n";
        return;
    }

    vector<vector<string>> allBlocks;
    vector<string> current;
    string line;

    while (getline(file, line))
    {
        current.push_back(line);
        if (line == "----")
        {
            allBlocks.push_back(current);
            current.clear();
        }
    }
    file.close();

    if (allBlocks.empty())
    {
        cout << "No orders on record.\n";
        return;
    }

    cout << "============================================================\n";
    cout << " YOUR ORDER DETAILS\n";
    cout << "============================================================\n";
    for (const auto &l : allBlocks.back())
        cout << l << "\n";
}

void FileManager::displayAllOrders()
{
    ifstream file(ORDER_FILE);
    if (!file.is_open())
    {
        cout << "No orders on record.\n";
        return;
    }
    cout << "============================================================\n";
    cout << " ALL ORDERS\n";
    cout << "============================================================\n";
    string line;
    while (getline(file, line))
        cout << line << "\n";
    file.close();
}
