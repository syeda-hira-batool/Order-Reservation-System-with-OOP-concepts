#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Person.h"
#include "Order.h"
#include "Menu.h"
#include <stdexcept>
using namespace std;

// ============================================================
// CLASS: Customer  (inherits from Person)
// ============================================================
class Customer : public Person
{
private:
    Order currentOrder;
    string deliveryAddress;
    bool hasDelivery;

public:
    Customer(const string &name, const string &email, long long phone, int orderID);

    Customer(const Customer &other);

    ~Customer();

    void displayInfo() const;
    string getRole() const;
    void displayDashboard() const;

    void placeOrder(const Menu &menu);
    void changeOrder(const Menu &menu);
    int getBill() const;
    void setDelivery(const string &address);
    int getFinalBill() const;

    const Order &getOrder() const;
    Order &getOrder();
    string getDeliveryAddress() const;
    bool isDelivery() const;
};

#endif
