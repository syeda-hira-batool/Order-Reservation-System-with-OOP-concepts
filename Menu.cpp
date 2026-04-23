#include "../include/Menu.h"
#include <stdexcept>

int Menu::totalMenuItems = 0;

Menu::Menu() {
    items = {
        MenuItem(1,  "Vegetable Chowmein",           500),
        MenuItem(2,  "Chicken Chowmein",              750),
        MenuItem(3,  "Mongolian Chicken",              520),
        MenuItem(4,  "Kung Pao Chicken",               820),
        MenuItem(5,  "Red Dragon Chicken",             700),
        MenuItem(6,  "Dynamite Prawns",                580),
        MenuItem(7,  "Mongolian Veal Ribs",            780),
        MenuItem(8,  "Beef Steak with Pepper Sauce",   540),
        MenuItem(9,  "Chocolate Lava Cake",            880),
        MenuItem(10, "Beef Steak",                     750)
    };
    totalMenuItems = (int)items.size();
}

void Menu::display() const {
    cout << *this;
}

const MenuItem& Menu::getItem(int number) const {
    if (number < 1 || number > (int)items.size())
        throw out_of_range("Invalid menu item number: " + to_string(number));
    return items[number - 1];
}

bool Menu::isValidItem(int number) const {
    return (number >= 1 && number <= (int)items.size());
}

int Menu::getSize() const {
    return (int)items.size();
}

int Menu::getTotalMenuItems() {
    return totalMenuItems;
}

ostream& operator<<(ostream& os, const Menu& m) {
    os << "============================================================\n";
    os << "                      MENU                                 \n";
    os << "============================================================\n";
    for (const auto& item : m.items)
        os << item << "\n";
    os << "============================================================\n";
    return os;
}
