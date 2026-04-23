#ifndef PERSON_H
#define PERSON_H
#include <iostream>
#include <string>
using namespace std;
class Person {
protected:
    
    string name;
    string email;
    string phoneNumber;

public:
   
    Person(const string& n, const string& e, string ph)
        : name(n), email(e), phoneNumber(ph) {}

    Person() : name(""), email(""), phoneNumber(0) {}

    virtual ~Person() {
    
    }

    void setName(const string& n)       { name = n; }
    void setEmail(const string& e)      { email = e; }
    void setPhoneNumber(string ph)   { phoneNumber = ph; }

    string    getName()        const { return name; }
    string    getEmail()       const { return email; }
    string getPhoneNumber() const { return phoneNumber; }

    virtual void displayInfo() const = 0;

    virtual string getRole() const { return "Person"; }
};

class IOrderable {
public:
    virtual void placeOrder()   = 0;
    virtual void cancelOrder()  = 0;
    virtual void changeOrder()  = 0;
    virtual ~IOrderable() {}
};

#endif
