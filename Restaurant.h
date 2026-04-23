#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <iostream>
#include <string>
using namespace std;

// ============================================================
//  CLASS: Restaurant
//  Demonstrates: Encapsulation, Constructors, Setters/Getters,
//                Const functions, Static members, Inline functions
// ============================================================
class Restaurant
{
private:
    string name;
    string address;
    string location;    // city / area
    string openingTime; // e.g. "09:00 AM"
    string closingTime; // e.g. "11:00 PM"
    string contactNumber;

    // Static member: only one restaurant instance needed
    static Restaurant *instance;

public:
    // Default constructor
    Restaurant()
        : name(""), address(""), location(""),
          openingTime(""), closingTime(""), contactNumber("") {}

    // Parameterized constructor with member initialization list
    Restaurant(const string &n, const string &addr,
               const string &loc, const string &open,
               const string &close, const string &contact)
        : name(n), address(addr), location(loc),
          openingTime(open), closingTime(close), contactNumber(contact) {}

    // Destructor
    ~Restaurant() {}

    // ---------- Setters ----------
    void setName(const string &n) { name = n; }
    void setAddress(const string &addr) { address = addr; }
    void setLocation(const string &loc) { location = loc; }
    void setOpeningTime(const string &t) { openingTime = t; }
    void setClosingTime(const string &t) { closingTime = t; }
    void setContactNumber(const string &c) { contactNumber = c; }

    // ---------- Getters (const functions) ----------
    string getName() const { return name; }
    string getAddress() const { return address; }
    string getLocation() const { return location; }
    string getOpeningTime() const { return openingTime; }
    string getClosingTime() const { return closingTime; }
    string getContactNumber() const { return contactNumber; }

    // Check if restaurant is currently open
    // (simple string comparison for demo purposes)
    bool isOpen() const
    {
        return (openingTime != "" && closingTime != "");
    }

    // Inline display header - shown at top of every screen
    inline void displayHeader() const
    {
        cout << "============================================================\n";
        cout << "              " << name << "\n";
        cout << "============================================================\n";
        cout << "  Address  : " << address << "\n";
        cout << "  Location : " << location << "\n";
        cout << "  Timings  : " << openingTime << " - " << closingTime << "\n";
        cout << "  Contact  : " << contactNumber << "\n";
        cout << "============================================================\n";
    }

    // Display full restaurant info
    void displayInfo() const
    {
        cout << "\n============================================================\n";
        cout << "               RESTAURANT INFORMATION\n";
        cout << "============================================================\n";
        cout << "  Name     : " << name << "\n";
        cout << "  Address  : " << address << "\n";
        cout << "  Location : " << location << "\n";
        cout << "  Opens    : " << openingTime << "\n";
        cout << "  Closes   : " << closingTime << "\n";
        cout << "  Contact  : " << contactNumber << "\n";
        cout << "  Status   : " << (isOpen() ? "OPEN" : "CLOSED") << "\n";
        cout << "============================================================\n";
    }

    // Static function - returns the one restaurant instance
    static Restaurant *getInstance()
    {
        if (instance == NULL)
        {
            instance = new Restaurant(
                "DAY 1 Palace Restaurant",
                "Shop 12, Block B, Main Boulevard",
                "Karachi, Pakistan",
                "11:00 AM",
                "11:00 PM",
                "03300123467");
        }
        return instance;
    }

    // Operator overloading: print restaurant info
    friend ostream &operator<<(ostream &os, const Restaurant &r)
    {
        os << r.name << " | " << r.location
           << " | " << r.openingTime << " - " << r.closingTime;
        return os;
    }
};

// Static member definition
Restaurant *Restaurant::instance = NULL;

#endif
