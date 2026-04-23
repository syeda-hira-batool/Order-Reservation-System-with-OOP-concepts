#ifndef MENUITEM_H
#define MENUITEM_H

#include <iostream>
#include <string>
using namespace std;
class MenuItem {
private:
    string name;
    int    price;
    int    itemNumber;

    
    static int totalMenuItems;

public:
  
    MenuItem() : name("Unknown"), price(0), itemNumber(0) {
        totalMenuItems++;
    }

    MenuItem(const string& n, int p, int num)
        : name(n), price(p), itemNumber(num) {
        totalMenuItems++;
    }

    MenuItem(const MenuItem& other)
        : name(other.name), price(other.price), itemNumber(other.itemNumber) {
        totalMenuItems++;
    }

    
    ~MenuItem() {
        totalMenuItems--;
    }

    
    void setName(const string& n)  { name = n; }
    void setPrice(int p)           { price = p; }
    void setItemNumber(int num)    { itemNumber = num; }

    string getName()      const { return name; }
    int    getPrice()     const { return price; }
    int    getItemNumber()const { return itemNumber; }

    static int getTotalMenuItems() { return totalMenuItems; }

    inline void display() const {
        cout << itemNumber << ". " << name;
   
        int spaces = 45 - (int)name.length();
        for (int i = 0; i < spaces; i++) cout << '-';
        cout << " " << price << " RS\n";
    }

    bool operator>(const MenuItem& other) const {
        return this->price > other.price;
    }

    bool operator==(const MenuItem& other) const {
        return this->itemNumber == other.itemNumber;
    }

    friend ostream& operator<<(ostream& os, const MenuItem& item);
};

int MenuItem::totalMenuItems = 0;

ostream& operator<<(ostream& os, const MenuItem& item) {
    os << "Item #" << item.itemNumber
       << " | " << item.name
       << " | " << item.price << " RS";
    return os;
}

#endif
