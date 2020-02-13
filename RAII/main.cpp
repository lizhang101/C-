#include <iostream>
/* Don't combine storing object in smart pointer with other statement.
*/
class Dog {};
class Trick {};
void train(shared_ptr<Dog> dog, Trick dogtrick) {}
Trick getTrick() {};
int main() {
    //fixme: train(shared_ptr<Dog>(new Dog), getTrick());
    // reason: if calling sequence is "new Dog" -> getTrick() -> shared_ptr, and getTrick throw an exception
    //          then the Dog object gets leak!!
    shared_ptr<Dog> pd(new Dog);
    train(pd, getTrick());
    return 0;
}
