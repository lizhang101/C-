#include <iostream>
// Problem: call base class copy constructor with a sub class object.
using namespace std;
class Dog {
public:
    virtual Dog* clone() {
        return new Dog(*this); //co-variant return type.
    }
    virtual void bark () {
        cout << "I'm a Dog" << endl;
    }

};

class YellowDog : public Dog {
public:
    virtual YellowDog* clone() {
        return new YellowDog(*this);
    }
    virtual void bark () {
        cout << "I'm a YellowDog" << endl;
    }
};

void play (Dog* d) { // d is a YellowDog by our usage
    Dog* c = new Dog(*d); // c is a Dog, copy of the Dog data in d
    c->bark();
    Dog* yellow = d->clone(); // now yellow is indeed a yellow dog.
    yellow->bark();
    delete c;
    delete yellow;
}

int main() {
    YellowDog ydog;
    play(&ydog);

    return 0;
}
