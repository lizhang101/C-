#include <iostream>
class Collar {};

//solution 1: implement assignment operator
class dog1 {
    Collar *pCollar;
    dog& operator=(const dog1& rhs) {
        // if omit this checking, first it may create a new object unnecessarily
        // and make followed code having less opportunities of making mistakes.
        if (this == &rhs) {
            return *this;
        }
        // delete the original pCollar only after a new Collar object is created.
        // If delete the pCollar before hand, and the new or Collar constructor throw an exception,
        // this will make pCollar pointing to an invalid address, and cause sigsegv when pCollar is used later or released again.
        Collar *pOrigCollar = pCollar;
        pCollar = new Collar(*rhs.pCollar);
        delete pOrigCollar;
        return *this;
    }

};
//solution 2: delegation
class dog2 {
    Collar *pCollar;
    dog2& operator=( const dog2& rhs) {
        *pCollar = *rhs.pCollar; // member by member copying or call Collar's operator=
        return *this;
    }

};
int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
