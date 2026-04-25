#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "OrderManager.h"
#include "BillingSystem.h"
#include "Restaurant.h"
using namespace std;

// --- Validation ---------------------------------------------------------------

bool isValidName(const string &name)
{
    if (name.empty() || name.length() < 2)
        return false;
    for (int i = 0; i < (int)name.size(); i++)
    {
        char c = name[i];
        if (!isalpha(c) && c != ' ' && c != '-' && c != '\'')
            return false;
    }
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
    for (int i = 0; i < (int)email.size(); i++)
        if (email[i] == ' ')
            return false;
    return true;
}

bool isValidPhone(const string &phone)
{
    string digits;
    for (int i = 0; i < (int)phone.size(); i++)
        if (isdigit(phone[i]))
            digits += phone[i];
    return digits.length() == 11;
}

// --- Duplicate Customer Check -------------------------------------------------

bool isAlreadyRegistered(const string &email, const string &phone)
{
    ifstream file("OrderFile.txt");
    if (!file)
        return false; // no file yet = no existing customers

    // Normalize phone: strip non-digits for comparison
    string phoneDigits = "";
    for (int i = 0; i < (int)phone.size(); i++)
        if (isdigit(phone[i]))
            phoneDigits += phone[i];

    string line;
    while (getline(file, line))
    {
        // Check email match
        if (line.find("Email: ") == 0)
        {
            string storedEmail = line.substr(7);
            if (storedEmail == email)
            {
                file.close();
                return true;
            }
        }
        // Check phone match (compare digits only)
        if (line.find("Phone: ") == 0)
        {
            string storedRaw = line.substr(7);
            string storedDigits = "";
            for (int i = 0; i < (int)storedRaw.size(); i++)
                if (isdigit(storedRaw[i]))
                    storedDigits += storedRaw[i];
            if (storedDigits == phoneDigits)
            {
                file.close();
                return true;
            }
        }
    }
    file.close();
    return false;
}

// --- Register Customer --------------------------------------------------------

Customer *registerCustomer()
{
    string name, email, phone;

    clearScreen();
    cout << "============================================================\n";
    cout << "                   CUSTOMER REGISTRATION\n";
    cout << "============================================================\n";

    cin.ignore();

    while (true)
    {
        cout << "  Enter your name  : ";
        getline(cin, name);
        if (isValidName(name))
            break;
        cout << "  [!] Invalid name. Use letters only.\n";
    }
    while (true)
    {
        cout << "  Enter your email : ";
        getline(cin, email);
        if (isValidEmail(email))
            break;
        cout << "  [!] Invalid email. Format: example@domain.com\n";
    }
    while (true)
    {
        cout << "  Enter your phone : ";
        getline(cin, phone);
        if (isValidPhone(phone))
            break;
        cout << "  [!] Invalid phone. Must be exactly 11 digits.\n";
    }

    // --- Duplicate check ---
    if (isAlreadyRegistered(email, phone))
    {
        clearScreen();
        cout << "============================================================\n";
        cout << "                  REGISTRATION FAILED\n";
        cout << "============================================================\n";
        cout << "  [!] A customer with this email or phone\n";
        cout << "      is already registered in our system.\n";
        cout << "  Please use different contact details.\n";
        cout << "============================================================\n";
        pauseScreen();
        return registerCustomer(); // ask again recursively
    }

    Customer *c = new Customer(name, email, phone);
    clearScreen();
    c->displayInfo();
    cout << "  Total customers registered: " << Customer::getTotalCustomers() << "\n";
    pauseScreen();
    return c;
}

// --- Choose Delivery ----------------------------------------------------------

DeliveryMethod *chooseDelivery(Customer *customer)
{
    clearScreen();
    cout << "============================================================\n";
    cout << "                   COLLECTION METHOD\n";
    cout << "============================================================\n";
    cout << "  1. Home Delivery\n";
    cout << "  2. Takeaway / Self Pickup  (Free)\n";
    cout << "------------------------------------------------------------\n";
    cout << "  Enter choice: ";
    int collectionChoice;
    cin >> collectionChoice;

    DeliveryMethod *dm = NULL;

    if (collectionChoice == 1)
    {
        string addr;
        cin.ignore();
        cout << "\n  Enter delivery address: ";
        getline(cin, addr);

        clearScreen();
        cout << "============================================================\n";
        cout << "                   DELIVERY OPTIONS\n";
        cout << "============================================================\n";
        cout << "  1.  PRIORITY    20 - 35 mins  ...........  +85 RS\n";
        cout << "      Emergency! Get your order fastest.\n";
        cout << "------------------------------------------------------------\n";
        cout << "  2.  STANDARD    25 - 40 mins  ...........  +50 RS\n";
        cout << "      Regular delivery speed.  [Default]\n";
        cout << "------------------------------------------------------------\n";
        cout << "  3.  SAVER       35 - 50 mins  ...........  +25 RS\n";
        cout << "      Slower but cheaper!\n";
        cout << "============================================================\n";
        cout << "  Enter speed (1/2/3): ";
        int speedChoice;
        cin >> speedChoice;

        if (speedChoice == 1)
            dm = new PriorityDelivery(addr);
        else if (speedChoice == 3)
            dm = new SaverDelivery(addr);
        else
            dm = new StandardDelivery(addr);

        customer->setDeliveryAddress(addr);
    }
    else
    {
        dm = new Pickup();
    }

    clearScreen();
    dm->displayMethod();
    pauseScreen();
    return dm;
}

// --- Bill Summary -------------------------------------------------------------

void showBillScreen(BillingSystem &billing, const Customer &customer)
{
    clearScreen();
    billing.validateBill();
    cout << billing;
    printCustomerBill(customer);
    pauseScreen();
}

// --- Main ---------------------------------------------------------------------

int main()
{
    srand((unsigned)time(NULL));

    clearScreen();
    Restaurant *restaurant = Restaurant::getInstance();
    restaurant->displayHeader();
    cout << "\n       WELCOME TO THE DAY 1 PALACE RESTAURANT ORDER SYSTEM\n";
    cout << "============================================================\n";
    pauseScreen();

    int again = 1;
    while (again == 1)
    {
        Customer *customer = registerCustomer();

        customer->placeOrder();

        // View order?
        clearScreen();
        cout << "\nView your full order? (1=Yes / 0=No): ";
        int viewChoice;
        cin >> viewChoice;
        if (viewChoice == 1)
        {
            clearScreen();
            customer->displayOrder();
            pauseScreen();
        }

        // Search by ID?
        clearScreen();
        cout << "\nSearch an order by ID? (1=Yes / 0=No): ";
        int searchChoice;
        cin >> searchChoice;
        if (searchChoice == 1)
        {
            int sid;
            cout << "Enter Order ID: ";
            cin >> sid;
            clearScreen();
            OrderManager::searchOrder(sid);
            pauseScreen();
        }

        // Change order?
        clearScreen();
        cout << "\nChange your order? (1=Yes / 0=No): ";
        int changeChoice;
        cin >> changeChoice;
        if (changeChoice == 1)
        {
            customer->changeOrder();
            clearScreen();
            customer->displayOrder();
            pauseScreen();
        }

        // Billing
        BillingSystem billing;
        billing.calculateBill(*customer);

        DeliveryMethod *dm = chooseDelivery(customer);
        billing.applyDeliveryCharge(dm->getCharge());

        customer->saveToFile();
        showBillScreen(billing, *customer);

        // Cancel?
        clearScreen();
        cout << "\nCancel an order? (1=Yes / 0=No): ";
        int cancelChoice;
        cin >> cancelChoice;
        if (cancelChoice == 1)
        {
            customer->cancelOrder();
            pauseScreen();
        }

        delete dm;

        clearScreen();
        cout << "============================================================\n";
        cout << "  Next customer? (1=Yes / 0=Exit): ";
        cin >> again;
        cout << "\n";

        delete customer; // delete AFTER printing next-customer prompt so count stays correct
    }

    clearScreen();
    cout << "============================================================\n";
    cout << "          Thank you for using our Order System!\n";
    cout << "  Total Customers Served: " << Customer::getTotalRegistered() << "\n";
    cout << "============================================================\n";
    pauseScreen();

    return 0;
}