#include "../include/Customer.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Customer::Customer(const string &name, const string &email,
                   long long phone, int orderID)
    : Person(name, email, phone),
      currentOrder(orderID),
      deliveryAddress(""),
      hasDelivery(false) {}

Customer::Customer(const Customer &other)
    : Person(other.name, other.email, other.phone),
      currentOrder(other.currentOrder),
      deliveryAddress(other.deliveryAddress),
      hasDelivery(other.hasDelivery) {}

Customer::~Customer() {}

void Customer::displayInfo() const
{
    cout << *this;
    cout << currentOrder;
}

string Customer::getRole() const
{
    return "Customer";
}

void Customer::displayDashboard() const
{
    Person::displayDashboard();
    cout << " Order ID  : " << currentOrder.getOrderID() << "\n";
    cout << " Items     : " << currentOrder.getItems().size() << "\n";
    cout << " Bill      : " << currentOrder.calculateBill() << " RS\n";
    cout << "------------------------------------------------------------\n";
}

void Customer::placeOrder(const Menu &menu)
{
    cout << menu;
    int count;
    cout << "How many items do you want to order? ";
    cin >> count;

    while (count > 0)
    {
        int choice;
        cout << "Enter item number: ";
        cin >> choice;
        try
        {
            const MenuItem &item = menu.getItem(choice);
            currentOrder.addItem(item);
            cout << "Added: " << item.getName() << "\n";
            count--;
        }
        catch (const out_of_range &e)
        {
            cout << "Invalid choice. Try again. (" << e.what() << ")\n";
        }
    }
}

void Customer::changeOrder(const Menu &menu)
{
    cout << "Changing order for ID: " << currentOrder.getOrderID() << "\n";
    currentOrder.clearItems();
    placeOrder(menu);
    cout << "Order updated successfully!\n";
}

int Customer::getBill() const
{
    return currentOrder.calculateBill();
}

void Customer::setDelivery(const string &address)
{
    deliveryAddress = address;
    hasDelivery = true;
}

int Customer::getFinalBill() const
{
    int bill = getBill();
    if (hasDelivery)
        bill += 50;
    return bill;
}

const Order &Customer::getOrder() const { return currentOrder; }
Order &Customer::getOrder() { return currentOrder; }
string Customer::getDeliveryAddress() const { return deliveryAddress; }
bool Customer::isDelivery() const { return hasDelivery; }
