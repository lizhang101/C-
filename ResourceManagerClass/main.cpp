#include <iostream>
/* problem:
 * If a class owns an object by pointer ( own means this class create and delete this object),
 * and then the objects of this class get copied, for example, pushed into a vector<ClassA>,
 * then because the copy is shallow copy by default, if the original object gets deleted, the copy object will have trouble.
 * Solutions:
 * 1. Customize the copy constructor and copy assignment.
 * 2. Disable the copy constructor & copy =, add clone function for deep copy, use vector<ClassA*>

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
