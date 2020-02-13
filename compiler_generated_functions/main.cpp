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
    ddog(std::string name) {}

    //disable the copy constructor
    ddog(const ddog& rhs) = delete;
private:
    //enforce the ddog object to be on heap, stay out of stack. For some embedded programming.
    //reason:   if put the object of this class on stack, you'll have to provide a public function like:
    //          void destroyMe(){ delete this; }
    //          Then, when user calls destroyMe(), object deleted; but it will be delete again when it's out of scope.
    ~ddog() { delete this; }


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

/* share_ptr can enforce both base and sub class's destructors called even the base destructor is not virtual.
 * this can be used when a class is derived from STL since all STL classes don't have virtual destructor.
*/

/* Dont leave exceptions out of destructor, because this may cause multiple pending exceptions and cause program to crash.
 * Solution1. catch the exception in the destructor.
 * Solution 2. create another function to throw exceptions called before destructor is called.
 */
typedef int MYEXCEPTION;

class dogExceptionSol1 {
public:
    ~dogException() {
        try {

        } catch (MYEXCEPTION e) {

        } catch (...) { // <<<<< catch all other exceptions.

        }
    }
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
