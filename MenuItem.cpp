#include "../include/MenuItem.h"

MenuItem::MenuItem() : itemNumber(0), name(""), price(0) {}

MenuItem::MenuItem(int num, const string& n, int p)
    : itemNumber(num), name(n), price(p) {}

string MenuItem::getName() const { return name; }
int MenuItem::getPrice() const { return price; }
int MenuItem::getItemNumber() const { return itemNumber; }

void MenuItem::setPrice(int p) { price = p; }

bool MenuItem::operator==(const MenuItem& other) const {
    return itemNumber == other.itemNumber;
}

ostream& operator<<(ostream& os, const MenuItem& item) {
    os << item.itemNumber << ". " << item.name
       << " ---------------------------------- " << item.price << " RS";
    return os;
}
