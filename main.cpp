#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "OrderManager.h"
#include "BillingSystem.h"
#include "Restaurant.h"
using namespace std;

// --- Validation Functions -----------------------------------------------------

bool isValidName(const string &name)
{
    if (name.empty() || name.length() < 2)
        return false;
    for (char c : name)
        if (!isalpha(c) && c != ' ' && c != '-' && c != '\'')
            return false;
    return true;
}

bool isValidEmail(const string &email)
{
    int atPos = email.find('@');
    if (atPos == (int)string::npos || atPos == 0)
        return false;

    int dotPos = email.find('.', atPos);
    if (dotPos == (int)string::npos || dotPos == atPos + 1)
        return false;

    if (dotPos >= (int)email.length() - 1)
        return false;

    for (char c : email)
        if (c == ' ')
            return false;

    return true;
}

bool isValidPhone(const string &phone)
{
    string digits;
    for (char c : phone)
        if (isdigit(c))
            digits += c;
    return digits.length() == 11;
}

// --- Register Customer --------------------------------------------------------

Customer *registerCustomer()
{
    string name, email, phone;

    cout << "\n============================================================\n";
    cout << "                   CUSTOMER REGISTRATION\n";
    cout << "============================================================\n";

    cin.ignore();

    // Name
    while (true)
    {
        cout << "Enter your name  : ";
        getline(cin, name);
        if (isValidName(name))
            break;
        cout << "  [!] Invalid name. Use letters only.\n";
    }

    // Email
    while (true)
    {
        cout << "Enter your email : ";
        getline(cin, email);
        if (isValidEmail(email))
            break;
        cout << "  [!] Invalid email. Format: example@domain.com\n";
    }

    // Phone
    while (true)
    {
        cout << "Enter your phone : ";
        getline(cin, phone);
        if (isValidPhone(phone))
            break;
        cout << "  [!] Invalid phone. Must contain exactly 11 digits.\n";
    }

    Customer *c = new Customer(name, email, phone);

    Person *p = c;
    p->displayInfo();

    cout << "\nTotal customers registered: "
         << Customer::getTotalCustomers() << "\n";
    return c;
}

// --- Choose Delivery ----------------------------------------------------------

DeliveryMethod *chooseDelivery(Customer *customer)
{
    int choice;
    cout << "\nSelect Collection Method:\n";
    cout << "  1. Home Delivery (+50 RS)\n";
    cout << "  2. Takeaway / Pickup (Free)\n";
    cout << "Enter choice: ";
    cin >> choice;

    DeliveryMethod *dm = NULL;

    if (choice == 1)
    {
        string addr;
        cin.ignore();
        cout << "Enter delivery address: ";
        getline(cin, addr);
        dm = new HomeDelivery(addr);
        customer->setDeliveryAddress(addr);
    }
    else
    {
        dm = new Pickup();
    }

    dm->displayMethod();
    return dm;
}

// --- Template Demo ------------------------------------------------------------

void showTemplateDemo(const Customer &c)
{
    if (c.getItemCount() == 0)
        return;

    cout << "  Total Bill: " << c.recursiveBillSum() << " RS\n";
}

// --- Main ---------------------------------------------------------------------

int main()
{
    srand((unsigned)time(NULL));

    Restaurant *restaurant = Restaurant::getInstance();
    restaurant->displayHeader();

    cout << "\n       WELCOME TO THE DAY 1 PALACE RESTAURANT ORDER SYSTEM\n";
    cout << "============================================================\n";

    int again = 1;
    while (again == 1)
    {

        Customer *customer = registerCustomer();

        cout << "\n--- PLACE YOUR ORDER ---\n";
        customer->placeOrder();

        int viewChoice;
        cout << "\nView your order? (1=Yes / 0=No): ";
        cin >> viewChoice;
        if (viewChoice == 1)
            customer->displayOrder();

        int searchChoice;
        cout << "\nSearch an order by ID? (1=Yes / 0=No): ";
        cin >> searchChoice;
        if (searchChoice == 1)
        {
            int sid;
            cout << "Enter Order ID to search: ";
            cin >> sid;
            OrderManager::searchOrder(sid);
        }

        int changeChoice;
        cout << "\nChange your order? (1=Yes / 0=No): ";
        cin >> changeChoice;
        if (changeChoice == 1)
        {
            customer->changeOrder();
            customer->displayOrder();
        }

        showTemplateDemo(*customer);

        BillingSystem billing;
        billing.calculateBill(*customer);

        DeliveryMethod *dm = chooseDelivery(customer);
        billing.applyDeliveryCharge(dm->getCharge());

        customer->saveToFile();

        billing.validateBill();
        cout << billing;

        printCustomerBill(*customer);

        int cancelChoice;
        cout << "\nCancel an order? (1=Yes / 0=No): ";
        cin >> cancelChoice;
        if (cancelChoice == 1)
        {
            customer->cancelOrder();
        }

        delete dm;
        delete customer;

        cout << "\nNext customer? (1=Yes / 0=Exit): ";
        cin >> again;
        cout << "\n";
    }

    cout << "============================================================\n";
    cout << "          Thank you for using our Order System!\n";
    cout << "  Final Customer Count: " << Customer::getTotalCustomers() << "\n";
    cout << "============================================================\n";

    return 0;
}