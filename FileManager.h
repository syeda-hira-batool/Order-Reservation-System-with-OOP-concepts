#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Customer.h"
#include <string>
#include <fstream>
#include <stdexcept>
using namespace std;

// ============================================================
// CLASS: FileManager
// ============================================================
class FileManager
{
private:
    static const string ORDER_FILE;
    static const string TEMP_FILE;

    FileManager() {}

public:
    static void saveCustomer(const Customer &c);
    static bool searchOrder(int orderID);
    static bool cancelOrder(int orderID);
    static void displayAllOrders();
    static void displayLastOrder();

    friend class Restaurant;
};

#endif
