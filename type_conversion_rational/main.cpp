#include <iostream>

/* Implicit type conversion is useful when creating numerical types of class,
 * such as a rational class.
 */
class Rational {
public:
    Rational(int numerator = 0, int dennominator = 1):
        num(numerator),den(dennominator) {}
    int num;  // Demo only, public data members are not recommended
    int den;
/*
    //(1)
    const Rational operator*(const Rational& rhs) {
        return Rational(num*rhs.num, den*rhs.den);
    }
    //(2)
    operator int () const { return num / den; }
*/
};
//(3)
//   Rational r3 = 3 * r1;
const Rational operator*( const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.num*rhs.num, lhs.den*rhs.den);
}


int main ()
{
    Rational r1 = 23; //implicit conversion, int to rational
    Rational r2 = r1 * 2; // with (1), 2->rational, then rational * rational
    Rational r3 = 3 * r1; // this won't work w/o (2), compiler see 3 first, then try to convert r1 to int.
                            // if we add (2), the previous line won't work. because now there 2 ways to convert
                            // 1 is 2 -> rational, then use rational *; another is r1 to int, then use global int *
                            // so it's ambiguous. So don't define 2-way conversion.
                            // The correct way is use (3)

}






