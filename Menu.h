#ifndef MENU_H
#define MENU_H

#include "MenuItem.h"
#include <iostream>
#include <string>
using namespace std;

class Menu
{
private:
    MenuItem items[10];
    int totalItems;

    static const int MAX_ITEMS = 10;

public:
    Menu() : totalItems(10)
    {
        items[0] = MenuItem("Vegetable Chowmein", 500, 1);
        items[1] = MenuItem("Chicken Chowmein", 750, 2);
        items[2] = MenuItem("Mongolian Chicken", 520, 3);
        items[3] = MenuItem("Kung Pao Chicken", 820, 4);
        items[4] = MenuItem("Red Dragon Chicken", 700, 5);
        items[5] = MenuItem("Dynamite Prawns", 580, 6);
        items[6] = MenuItem("Mongolian Veal Ribs", 780, 7);
        items[7] = MenuItem("Beef Steak with Pepper Sauce", 540, 8);
        items[8] = MenuItem("Chocolate Lava Cake", 880, 9);
        items[9] = MenuItem("Beef Steak", 750, 10);
    }

    void displayMenu() const
    {
        cout << "============================================================\n";
        cout << "                     RESTAURANT MENU\n";
        cout << "============================================================\n";
        for (int i = 0; i < totalItems; i++)
        {
            items[i].display();
        }
        cout << "============================================================\n";
    }
    const MenuItem *getItem(int num) const
    {
        if (num < 1 || num > totalItems)
            return nullptr;
        return &items[num - 1];
    }

    int getPriceByNumber(int num) const
    {
        const MenuItem *item = getItem(num);
        if (item)
            return item->getPrice();
        return -1;
    }
    string getNameByNumber(int num) const
    {
        const MenuItem *item = getItem(num);
        if (item)
            return item->getName();
        return "Unknown";
    }

    int getTotalItems() const { return totalItems; }

    static int getMaxItems() { return MAX_ITEMS; }
};

#endif
