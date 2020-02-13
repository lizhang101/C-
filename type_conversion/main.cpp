#include <iostream>
#include <string>
using namespace std;

/*
 * Category C: Implicit User Defined Type Conversion
 *
 * Defined inside class (user defined type)
 *
 * There are 2 methods to define implicit user defined type conversion:
 * Method 1: Use constructor that can accept a single parameter.
 *    - convert other types of object into your class
 * Method 2: Use the type conversion function
 *    - convert an object of your class into other types
 */
class dog {
public:
    dog(string name) {m_name = name; }  // NO explicit, this is constructor + implicit + explicit converter.
    explicit dog(int age) { m_age = age; }
    // If you only want to define a constructor, and no implicit type
    // conversion, always put "explicit" before the constructor to
    // avoid inadvertent type conversion.
    string getName() {return m_name;}
    //method 2, convert to another type
    operator string() const {return m_name; }
private:
    string m_name;
    int m_age;
};

int main ()
{
    string dogname = "Bob";
    dog dog1 = dogname; // <<< implicit type conversion here.
    printf("My name is %s.\n", dog1.getName().c_str());
    printf("My name is %s.\n", dog1);
    return 0;
}
/*
OUTPUT:
My name is Bob
*/


/* PRINCIPLE: make interface easy to use correctly and hard to use incorrectly.
 * How hard is enough?  Ideally, uncompilable.
 *
 * General guideline:
 *    1. Avoid defining seemingly unexpected conversion.
 *    2. Avoid defining two-way implicit conversion.
 */


//My Notes:
//operator string () const { return m_name; }
// string dog2 = dog1;
//
