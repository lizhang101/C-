#include <iostream>
/*
 compiler generated functions:
 1. copy constructor
 2. copy assignment operator
 3. destructor
 4. default destructor (only if no constructor declared)
 */

class ddog {
    //the reference will stop compiler to generate the default constructor.
    //And also this makes ddog incompatible with STL lib.
    //fixme: string& m_name;

public:
    /* C++11 */
    // even you defined ddog constructor, you can still use compiler's generated constructor in this way.
    ddog() = default;
    ddog(string name) {}

    //disable the copy constructor
    ddog(const ddog& rhs) = delete;
private:
    //enforce the ddog object to be on heap, stay out of stack. For some embedded programming.
    ~ddog() {}


};
//with an empty class ddog{}, it's equivalent to :
class dog {
public:
    dog(const dog& rhs) { } // member by member initialization
    dog& operator=(const dog& rhs) {} // member by member copying

    dog() {} // 1. call base class's default constructor.
             // 2. call data member's default constructor.

    ~dog() {}   // 1. call base class destructor
                // 2. call members' destructor.
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
