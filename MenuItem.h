#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>
#include <iostream>
using namespace std;

// ============================================================
// CLASS: MenuItem
// ============================================================
class MenuItem {
private:
    int itemNumber;
    string name;
    int price;

public:

    MenuItem();
    MenuItem(int num, const string& name, int price);

    string getName() const;
    int getPrice() const;
    int getItemNumber() const;

    void setPrice(int p);

    bool operator==(const MenuItem& other) const;

    friend ostream& operator<<(ostream& os, const MenuItem& item);
};

#endif
