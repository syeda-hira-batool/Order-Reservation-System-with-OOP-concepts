#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Person.h"
#include "Menu.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Customer : public Person, public IOrderable
{
private:
    int orderID;
    string orderedItems[20];
    int orderedPrices[20];
    int itemCount;
    string deliveryAddress;
    bool isDelivery;

    static int totalCustomers;
    static int nextOrderID;

    void generateOrderID()
    {
        orderID = nextOrderID++;
    }

public:
    Customer(const string &n, const string &e, string ph)
        : Person(n, e, ph),
          orderID(0), itemCount(0), isDelivery(false)
    {
        generateOrderID();
        totalCustomers++;
    }

    Customer() : Person(), orderID(0), itemCount(0), isDelivery(false)
    {
        generateOrderID();
        totalCustomers++;
    }

    ~Customer() override
    {
        totalCustomers--;
    }

    void placeOrder() override;
    void cancelOrder() override;
    void changeOrder() override;

    void displayInfo() const override
    {
        cout << "============================================================\n";
        cout << "                   CUSTOMER INFORMATION\n";
        cout << "============================================================\n";
        cout << "  Name     : " << name << "\n";
        cout << "  Email    : " << email << "\n";
        cout << "  Phone    : " << phoneNumber << "\n";
        cout << "  OrderID  : " << orderID << "\n";
        cout << "  Role     : " << getRole() << "\n";
        cout << "============================================================\n";
    }

    string getRole() const override { return "Customer"; }

    int getOrderID() const { return orderID; }
    int getItemCount() const { return itemCount; }

    void addItem(const string &itemName, int price)
    {
        if (itemCount < 20)
        {
            orderedItems[itemCount] = itemName;
            orderedPrices[itemCount] = price;
            itemCount++;
        }
    }

    void setDeliveryAddress(const string &addr)
    {
        deliveryAddress = addr;
        isDelivery = true;
    }

    string getDeliveryAddress() const { return deliveryAddress; }
    bool getIsDelivery() const { return isDelivery; }

    int recursiveBillSum(int index = 0) const
    {
        if (index == itemCount)
            return 0;
        return orderedPrices[index] + recursiveBillSum(index + 1);
    }

    void displayOrder() const
    {
        cout << "============================================================\n";
        cout << "                     ORDER DETAILS\n";
        cout << "============================================================\n";
        cout << "  OrderID         : " << orderID << "\n";
        cout << "  Name            : " << name << "\n";
        for (int i = 0; i < itemCount; i++)
        {
            cout << "  Item " << (i + 1) << "           : "
                 << orderedItems[i] << " - " << orderedPrices[i] << " RS\n";
        }
        if (itemCount == 0)
            cout << "  (No items ordered yet)\n";

        if (isDelivery)
        {
            cout << "  Collection      : Home Delivery\n";
            cout << "  Address         : " << deliveryAddress << "\n";
        }
        else
        {
            cout << "  Collection      : Takeaway / Pickup\n";
        }
        cout << "============================================================\n";
    }

    void saveToFile() const
    {
        ofstream ptr("OrderFile.txt", ios::app);
        if (!ptr)
        {
            cerr << "Error opening file!\n";
            return;
        }

        ptr << "Name: " << name << "\n";
        ptr << "Email: " << email << "\n";
        ptr << "Phone: " << phoneNumber << "\n";
        ptr << "OrderID: " << orderID << "\n";
        for (int i = 0; i < itemCount; i++)
        {
            ptr << "Item: " << orderedItems[i] << "\n";
        }
        if (isDelivery)
        {
            ptr << "Collection: Home Delivery\n";
            ptr << "Address: " << deliveryAddress << "\n";
        }
        else
        {
            ptr << "Collection: Takeaway / Pickup\n";
        }
        ptr << "---\n";
        ptr.close();
    }

    static int getTotalCustomers() { return totalCustomers; }

    friend class BillingSystem;
    friend void printCustomerBill(const Customer &c);
};

// --- Static Member Definitions ------------------------------------------------
int Customer::totalCustomers = 0;
int Customer::nextOrderID = 1000;

// --- Friend Function ----------------------------------------------------------
void printCustomerBill(const Customer &c)
{
    cout << "\n[Friend Function] Bill Report for OrderID: " << c.orderID << "\n";
    for (int i = 0; i < c.itemCount; i++)
    {
        cout << "  " << c.orderedItems[i]
             << " .............. " << c.orderedPrices[i] << " RS\n";
    }
    cout << "  Total: " << c.recursiveBillSum() << " RS\n";
}

#endif
