#ifndef BILLINGSYSTEM_H
#define BILLINGSYSTEM_H
#include "Customer.h"
#include <iostream>
#include <stdexcept>
using namespace std;

class DeliveryMethod
{
protected:
    string methodName;
    int deliveryCharge;

public:
    DeliveryMethod(const string &name, int charge)
        : methodName(name), deliveryCharge(charge) {}

    virtual ~DeliveryMethod() {}

    virtual int getCharge() const { return deliveryCharge; }

    virtual void displayMethod() const
    {
        cout << "  Delivery Method : " << methodName << "\n";
        cout << "  Delivery Charge : " << deliveryCharge << " RS\n";
    }

    virtual string getMethodName() const { return methodName; }
};

class HomeDelivery : public DeliveryMethod
{
private:
    string address;

public:
    HomeDelivery(const string &addr)
        : DeliveryMethod("Home Delivery", 50), address(addr) {}

    void displayMethod() const override
    {
        DeliveryMethod::displayMethod();
        cout << "  Address         : " << address << "\n";
    }

    string getAddress() const { return address; }
};

class Pickup : public DeliveryMethod
{
public:
    Pickup() : DeliveryMethod("Pickup", 0) {}

    void displayMethod() const override
    {
        cout << "  Delivery Method : " << methodName << "\n";
        cout << "  (No delivery charge - self pickup)\n";
    }

    int getCharge() const override { return 0; }
};

template <typename T>
T findMax(T arr[], int size)
{
    if (size <= 0)
        throw invalid_argument("Array size must be > 0");
    T maxVal = arr[0];
    for (int i = 1; i < size; i++)
    {
        if (arr[i] > maxVal)
            maxVal = arr[i];
    }
    return maxVal;
}

template <typename T>
T findMin(T arr[], int size)
{
    if (size <= 0)
        throw invalid_argument("Array size must be > 0");
    T minVal = arr[0];
    for (int i = 1; i < size; i++)
    {
        if (arr[i] < minVal)
            minVal = arr[i];
    }
    return minVal;
}

class BillingSystem
{
private:
    int subtotal;
    int deliveryCharge;
    int total;
    bool calculated;

public:
    BillingSystem() : subtotal(0), deliveryCharge(0), total(0), calculated(false) {}

    void calculateBill(const Customer &c)
    {

        subtotal = 0;
        for (int i = 0; i < c.itemCount; i++)
        {
            subtotal += c.orderedPrices[i];
        }
        calculated = true;
        total = subtotal;
    }

    void applyDeliveryCharge(int charge)
    {
        deliveryCharge = charge;
        total = subtotal + deliveryCharge;
    }

    BillingSystem &operator+=(int extraCharge)
    {
        total += extraCharge;
        return *this;
    }
    friend ostream &operator<<(ostream &os, const BillingSystem &bs)
    {
        os << "============================================================\n";
        os << "                       BILL SUMMARY\n";
        os << "============================================================\n";
        os << "  Subtotal        : " << bs.subtotal << " RS\n";
        os << "  Delivery Charge : " << bs.deliveryCharge << " RS\n";
        os << "  TOTAL           : " << bs.total << " RS\n";
        os << "============================================================\n";
        return os;
    }

    int getTotal() const { return total; }
    int getSubtotal() const { return subtotal; }

    void validateBill() const
    {
        try
        {
            if (!calculated)
            {
                throw runtime_error("Bill not calculated yet!");
            }
            if (total < 0)
            {
                throw invalid_argument("Bill cannot be negative!");
            }
            if (total > 50000)
            {
                throw overflow_error("Bill exceeds maximum allowed amount!");
            }
            cout << "  [Bill validated successfully]\n";
        }
        catch (const invalid_argument &e)
        {
            cerr << "  Argument Error: " << e.what() << "\n";
        }
        catch (const overflow_error &e)
        {
            cerr << "  Overflow Error: " << e.what() << "\n";
        }
        catch (const runtime_error &e)
        {
            cerr << "  Runtime Error: " << e.what() << "\n";
        }
        catch (...)
        {
            cerr << "  Unknown billing error occurred!\n";
        }
    }
};

#endif
