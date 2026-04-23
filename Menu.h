#ifndef MENU_H
#define MENU_H

#include "MenuItem.h"
#include <vector>
#include <iostream>
using namespace std;

// ============================================================
// CLASS: Menu
// ============================================================
class Menu {
private:
    vector<MenuItem> items;       
    static int totalMenuItems;   

public:
    Menu();  

    void display() const;
    const MenuItem& getItem(int number) const;  
    bool isValidItem(int number) const;
    int getSize() const;

    static int getTotalMenuItems();

    friend ostream& operator<<(ostream& os, const Menu& m);
};

#endif
