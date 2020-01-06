#include <iostream>
using namespace std;
/* USE CONST WHENEVER POSSIBLE */

// -------const used with functions.
class Dog {
    int age;
    string name;
public:
    Dog() {
        age = 3;
        name = "dummy";
    }
    // const string getName() doesn't make sense.
    // Since this function is called by another const function, it has to be a const function too.
    const string & getName() const { return name; }

    //const parameters
    // overloaded functions, which one being called depends on the constness of the argument.
    void setAge(const int& a) {
        age = a;
        cout << "set (const) " << endl;
    }
    void setAge(int& a) {
        age = a;
        cout << "set (non-const)" << endl;
    }

    //----const functions
    // They are 2 functions. when Dog object is const, the const function (first 1) will be called.
    // Otherwise, the 2nd one will be called.
    void printName() const { cout << getName() << " const function" << endl; }
    void printName() { cout << getName() << " non-const function" << endl; }
};

int main() {
    //------------const cast
    const int j = 10;
    //FIXME: j = 9; // illegal
    const_cast<int&>(j) = 8; // but the j will still be 10 after this statement !
    cout << "j:" << j << endl;

    int i = 10;
    //FIXME: static_cast<const int &>(i) = 11; //invalid

    //--------const data, const pointer

    const int * p0 = &i; // data is const
    int * const p1 = &i; // pointer is const
    // if const is at left of the start, the data is constant.
    // if const is at the right of the star, the pointer is constant.


    //----------
    Dog d;
    d.printName();
    const Dog const_dog;
    const_dog.printName();

    int age = 10;
    d.setAge(age);
    d.setAge(100);

    return 0;
}
