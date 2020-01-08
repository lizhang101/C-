#include <iostream>
#include "singleton.h"

/*
 * Problem: say we have 2 classes, Dog and Cat. and we have 2 global objects, cat and dog. In Dog and Cat constructor,
 * we call the global cat or dog functions.
 * Since the order of global objects initialization is not defined, if cat call dog's function in constructor and dog is constructed after cat,
 * then program will crash.
 * The solution is to use singleton. Note the assumption is we only need to have one dog and one cat.
*/

class Dog;

class Singleton {
    static Dog *pDog_;
    static Cat *pDog_;
public:
    static Dog* getDog() {
        if (!pDog_) {
            return new Dog();
        } else {
            return pDog_;
        }
    }
    static Cat* getCat() {
        if (!pCat_) {
            return new Cat();
        } else {
            return pCat_;
        }
    }
    ~Singleton() {
        delete pCat_;
        delete pDog_;
    }
};
class Cat {
public:
    void meow() {};
}

class Dog  {
public:
    void bark() {};
    Dog () {
        //use singleton to call cat.
        singleton::getCat()->meow();
    }
};
//-------cpp
Dog *Singleton::pDog_ = nullptr;
Cat *Singleton::pCat_ = nullptr;

int main() {
    Singleton s; // make sure dog and cat will be released after usage.
    Singleton::GetDog->bark();

    return 0;
}
