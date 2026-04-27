#ifndef BILLINGSYSTEM_H
#define BILLINGSYSTEM_H
#include "Customer.h"
#include <iostream>
#include <stdexcept>
using namespace std;

// ============================================================
//  BASE: DeliveryMethod
// ============================================================
class DeliveryMethod
{
protected:
    string methodName;
    int    deliveryCharge;
    string estimatedTime;

public:
    DeliveryMethod(const string &name, int charge, const string &time)
        : methodName(name), deliveryCharge(charge), estimatedTime(time) {}

    virtual ~DeliveryMethod() {}

    virtual int    getCharge()        const { return deliveryCharge; }
    virtual string getMethodName()    const { return methodName;     }
    string         getEstimatedTime() const { return estimatedTime;  }

    virtual void displayMethod() const
    {
        cout << "------------------------------------------------------------\n";
        cout << "  Delivery Method : " << methodName    << "\n";
        cout << "  Estimated Time  : " << estimatedTime << "\n";
        if (deliveryCharge == 0)
            cout << "  Delivery Charge : Free\n";
        else
            cout << "  Delivery Charge : +" << deliveryCharge << " RS\n";
        cout << "------------------------------------------------------------\n";
    }
};

// ---- Priority (Emergency) ---------------------------------------------------
class PriorityDelivery : public DeliveryMethod
{
    string address;
public:
    PriorityDelivery(const string &addr)
        : DeliveryMethod("Priority / Emergency", 85, "20 - 35 mins"), address(addr) {}

    void displayMethod() const
    {
        cout << "------------------------------------------------------------\n";
        cout << "  *** PRIORITY / EMERGENCY DELIVERY ***\n";
        cout << "  Estimated Time  : " << estimatedTime << "\n";
        cout << "  Delivery Charge : +" << deliveryCharge << " RS\n";
        cout << "  Address         : " << address << "\n";
       // cout << "  [Top priority arrives fastest!]\n";
        cout << "------------------------------------------------------------\n";
    }
};

// ---- Standard ---------------------------------------------------------------
class StandardDelivery : public DeliveryMethod
{
    string address;
public:
    StandardDelivery(const string &addr)
        : DeliveryMethod("Standard Delivery", 50, "25 - 40 mins"), address(addr) {}

    void displayMethod() const
    {
        cout << "------------------------------------------------------------\n";
        cout << "  Delivery Method : " << methodName    << "\n";
        cout << "  Estimated Time  : " << estimatedTime << "\n";
        cout << "  Delivery Charge : +" << deliveryCharge << " RS\n";
        cout << "  Address         : " << address << "\n";
        cout << "------------------------------------------------------------\n";
    }
};

// ---- Saver ------------------------------------------------------------------
class SaverDelivery : public DeliveryMethod
{
    string address;
public:
    SaverDelivery(const string &addr)
        : DeliveryMethod("Saver Delivery", 25, "35 - 50 mins"), address(addr) {}

    void displayMethod() const
    {
        cout << "------------------------------------------------------------\n";
        cout << "  Delivery Method : " << methodName    << "\n";
        cout << "  Estimated Time  : " << estimatedTime << "\n";
        cout << "  Delivery Charge : +" << deliveryCharge << " RS\n";
        cout << "  Address         : " << address << "\n";
     //   cout << "  [Budget option slower but saves money!]\n";
        cout << "------------------------------------------------------------\n";
    }
};

// ---- Pickup -----------------------------------------------------------------
class Pickup : public DeliveryMethod
{
public:
    Pickup() : DeliveryMethod("Takeaway / Self Pickup", 0, "Ready in 15 - 20 mins") {}

    void displayMethod() const
    {
        cout << "------------------------------------------------------------\n";
        cout << "  Delivery Method : " << methodName    << "\n";
        cout << "  Estimated Time  : " << estimatedTime << "\n";
        cout << "  Delivery Charge : Free\n";
        cout << "------------------------------------------------------------\n";
    }

    int getCharge() const { return 0; }
};

// Keep HomeDelivery as alias for backward compat
class HomeDelivery : public StandardDelivery
{
public:
    HomeDelivery(const string &addr) : StandardDelivery(addr) {}
};

// ---- Templates --------------------------------------------------------------
//These are C++ Template Functions — a way to write one function that works with any data type
template <typename T>
T findMax(T arr[], int size)
{
    if (size <= 0) throw invalid_argument("Array size must be > 0");
    T maxVal = arr[0];
    for (int i = 1; i < size; i++) if (arr[i] > maxVal) maxVal = arr[i];
    return maxVal;
}

template <typename T>
T findMin(T arr[], int size)
{
    if (size <= 0) throw invalid_argument("Array size must be > 0");
    T minVal = arr[0];
    for (int i = 1; i < size; i++) if (arr[i] < minVal) minVal = arr[i];
    return minVal;
}

// ============================================================
//  CLASS: BillingSystem
// ============================================================
class BillingSystem
{
private:
    int  subtotal;
    int  deliveryCharge;
    int  total;
    bool calculated;

public:
    BillingSystem() : subtotal(0), deliveryCharge(0), total(0), calculated(false) {}

    void calculateBill(const Customer &c)
    {
        subtotal = 0;
        for (int i = 0; i < c.itemCount; i++) subtotal += c.orderedPrices[i];
        calculated = true;
        total = subtotal;
    }

    void applyDeliveryCharge(int charge)
    {
        deliveryCharge = charge;
        total = subtotal + deliveryCharge;
    }

    BillingSystem &operator+=(int extraCharge) { total += extraCharge; return *this; }

    friend ostream &operator<<(ostream &os, const BillingSystem &bs)
    {
        os << "============================================================\n";
        os << "                       BILL SUMMARY\n";
        os << "============================================================\n";
        os << "  Subtotal        : " << bs.subtotal       << " RS\n";
        os << "  Delivery Charge : " << bs.deliveryCharge << " RS\n";
        os << "  TOTAL           : " << bs.total          << " RS\n";
        os << "============================================================\n";
        return os;
    }

    int  getTotal()    const { return total;    }
    int  getSubtotal() const { return subtotal; }

    void validateBill() const
    {
        try
        {
            if (!calculated)    throw runtime_error("Bill not calculated yet!");
            if (total < 0)      throw invalid_argument("Bill cannot be negative!");
            if (total > 50000)  throw overflow_error("Bill exceeds maximum allowed amount!");
            cout << "  [Bill validated successfully]\n";
        }
        catch (const invalid_argument &e) { cerr << "  Argument Error: " << e.what() << "\n"; }
        catch (const overflow_error    &e) { cerr << "  Overflow Error: " << e.what() << "\n"; }
        catch (const runtime_error     &e) { cerr << "  Runtime Error: "  << e.what() << "\n"; }
        catch (...)                        { cerr << "  Unknown billing error occurred!\n";     }
    }
};

#endif
