* functions that don't use any class members will treated as static functions by compiler.

```c++
class dog {
    public:
    void bark() { cout << "bark" << endl;}
};

int main(void) {
dog *pd = nullptr;   
//even pd points to no object, this code can still run and print bark  
pd->bark();   
}
```

* Only static_cast can cast on objects, others can only work on pointer/reference.

* share_ptr can enforce both base and derived destructor to be called, even if base class destructor is not virtual.
  This is useful for classes derived from STL. Or, use **Private Inheritance**
  
* Use composition whenever possible, use priviate inheritance when have to. Because:
    * composition: can have multiple instances, can switch classes and objects.
    * private inheritance: want to override a function in the reused class.
    example:
```c++
class Ring {
    public:
    virtual void tremble() {};// each dog can have different tremble implementation
    virutal void tinkle() {
        tremble();
    }
    ~Ring() {} // even this destructor is not virtual, it will be called when a dog is destroyed.
};
//Now, each dog can have ONLY ONE ring and you can't change to another type of ring or object dynamically.
class Dog : private Ring {
    using Ring::tinkle;
};
```

* Never override non-virtual functions.
```c++
class dog {
    public:
    void  bark (int age) { } //non-virtual. will be shadowed if the derived class override the 2nd virtual bark(string)
    virtual void bark(const string& msg) {} // 
};
```

* Never redefine default parameters for virtual function.

* NVI (Non-virtual Interface Idiom/Template Design Pattern)
    * base class defines a "generic" algorithm that's used by derived classes. (Template). 
    * The generic function calls some virtual private functions.
    * The derived classes override those virtual private functions.
    
* Alternatives for Virtual function.
    * function pointer
    * functor
    * Strategy Class    

* CRTP (Curiously Recurring Template Pattern / Static Polymorphism)
    * Use class Template as the base class.
```c++
struct TreeNode {TreeNode *left, *right;};

template <typename T>
class Generic_Parser {
public:
    void parse_preorder(TreeNode* node) {
        if (node) {
            process_node(node);
            parse_preorder(node->left);
            parse_preorder(node->right);
        }
    }
    void process_node(TreeNode* node) {
        static_cast<T*>(this)->process_node(node);
    }
};

class EmployeeChart_Parser : public Generic_Parser<EmployeeChart_Parser> {
public:
    void process_node(TreeNode* node) {
        cout << "Customized process_node() for EmployeeChart.\n";
    }
};

int main() {
    ...
    EmployeeChart_Parser ep;
    ep.parse_preorder(root);
    ...
}

```

* Multiple Inheritance
    * Use virtual inheritance: Not good, has initialization issue. The most derived class has to initialize the base class explicitly.
    * use PAC (Pure Abstract Class).
        * A class contains only public pure virtual functions: No data, No concrete functions, No protected, Private Functions.
        * A Abstract class is the class that has at least one pure virtual class.

* Interface Inheritance vs. Implementation Inheritance
    * Minimize implementation in base classes.
    * Minimize the level of hierarchies in implementation inheritance.
    * If base and derived class share some common functions, put them into another helper class.
    
* Code reuse 
```c++
//---------------------------------
//Bad example: reusing by inheritance
// first, don't say class BaseDog
class Dog { ... // some common activity functions. };
class BullDog : public Dog{};
class ShepherDog : public Dog{};

//Better: Use composition and helper functions.
class ActivityManager { // common activities };
class Dog { } // PAC
class BullDog : public Dog{
    ActivityManager *pActMngr;
    ... // call the common activities through pActMngr.
};
class ShepherDog : public Dog{ 
    ActivityManager *pActMngr;
    ... // call the common activities through pActMngr.
};

//Further more, you can have bind to different activities dynamically:
class OutdoorActivityManager : public ActivityManager{ ... };
class IndoorActivityManager : public ActivityManager{ ... };

```
Another Example:
```c++
// Inheritance Example:
class Dog {
    ...
    void eatBiscuit() {... }
};

class BullDog : public Dog {
    ...
};

class ShepherdDog : public Dog {
    ...
};

// Composition Example: Inherit only from pure abstract class:
class ActivityManager {
    void eatBiscuit() { ... }
};

class Dog {
    ...
    virtual void eatBiscuit() = 0;
};

class BullDog : public Dog {
    ActivityManager* pActMngr;
    ...
    void eatBiscuit() { pActMngr->eatBiscuit(); }
};

class ShepherdDog : public Dog {
    ActivityManager* pActMngr;
    ...
    void eatBiscuit() { pActMngr->eatBiscuit(); }
};



```

* PImpl  
Using nested class.  
Minimize header file exposure. Reduce build time.

```c++
#include <iostream>
#include <memory>
class widget {
    class impl;
    std::unique_ptr<impl> pimpl;
public:
    void draw(); // public API that will be forwarded to the implementation
    widget(int); // defined in the implementation file
    ~widget();   // defined in the implementation file, where impl is a complete type
    widget(widget&&) = default;
    widget(const widget&) = delete;
    widget& operator=(widget&&); // defined in the implementation file
    widget& operator=(const widget&) = delete;
};

class widget::impl {
    int n; // private data
public:
    void draw(const widget& w) { std::cout << "Pimpl Draw" << std::endl; }
    impl(int n) : n(n) {}
};

void widget::draw() { pimpl->draw(*this); }
widget::widget(int n) : pimpl{std::make_unique<impl>(n)} {}
widget::~widget() = default;
widget& widget::operator=(widget&&) = default;

int main() {
    widget w(10);
    w.draw();
    return 0;
}

```

* RAII  

* If a class owns a resource by raw pointers, when it's copied, like pushed into a STL container, it needs to:
    * override copy constructor, copy assignment operator for deep copy.
    * Or, disable copy constructor, copy assignment and use a clone function.
    
* Clone: 
```c++
class Interface { 
    virtual unique_ptr<Interface> clone() const = 0;
};

class Implementation : public Interface {
    unique_ptr<Interface> clone() const override {}
    //Implementation* clone() const override {} // if use raw pointer, then users must pay attention to delete this object. so better to use unique_ptr.
};
``` 
If using multiple inheritance, it's better to use different clone function names for different interfaces.

* rvalue reference
```c++
class MyVector{
    public:
    int size = 10;
    void overload(const MyVector& rhs);
    void overload(const MyVector&& rhs);

    //-----------------------
    // Here, to show printIt("aaa") will call 2nd printIt first, then the internal printIt will call the first one.
    // use perfect forward to keep it as rvalue reference.
    void printIt(string& s) {
        cout << "lvalue print\n";
    }
    void printIt(string&& s) {
        cout << "rvalue print\n";
        printIt(s);
    }
    //--------------

    MyVector(const MyVector& rhs) {
        cout << "copy\n";
    }
    MyVector(const MyVector&& rhs) {
        cout <<"move";
        size = rhs.size;
        overload(rhs);
    }
    MyVector() {
        size = 20;
        cout << "default\n";
    }
};
void MyVector::overload(const MyVector& rhs) {
    cout << "by lvalue\n";
} 
void MyVector::overload(const MyVector&& rhs) {
    cout << "by rvalue\n";
} 
int main() {
    MyVector v0;
    v0.printIt("aaa");
    cout <<"test\n";
    MyVector v1 = move(MyVector()); // will call move constructor.
    cout <<"test1\n";
}
```
```
output:
default
rvalue print
lvalue print
test
default
moveby lvalue
test1
```

* all STL containers have move constructor. return by value is optimized by compiler.

* For vector->push(A): if A has move constructor, A will be moved instead of copy. otherwise, copy constructor is called.

* universal Reference
```c++
template< typename T >
void relay(T&& arg ) {
    ...
}

// T&& variable is intialized with rvalue => rvalue reference
//relay(9); =>  T = int&& =>  T&& = int&& && = int&&

// T&& variable is intialized with lvalue => lvalue reference
//        relay(x); =>  T = int&  =>  T&& = int& && = int&

// T&& is Universal Reference: rvalue, lvalue, const, non-const, etc...
// Conditions:
// 1. T is a template type.
// 2. Type deduction (reference collasing) happens to T.
//    - T is a function template type, not class template type.
//
```

* self assignment
```c++
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
```

* Static polymorphism
```c++
/*
 * 1. is-a relationship between base class and derived class
 * 2. Base class defines a "generic" algorithm that's used by derived class (parse_preorder)
 * 3. The "generic" algorithm is customized by the derived class
 */
// TMP: Template Metaprogramming
template <typename T> class Generic_Parser {
public:
    void parse_preorder(TreeNode* node) {
        if (node) {
            process_node(node);
            parse_preorder(node->left);
            parse_preorder(node->right);
        }
    }
    void process_node(TreeNode* node) {
        static_cast<T*>(this)->process_node(node);
    }
};

class EmployeeChart_Parser : public Generic_Parser<EmployeeChart_Parser> {
public:
    void process_node(TreeNode* node) {
        cout << "Customized process_node() for EmployeeChart.\n";
    }
};

int main() {
    ...
    EmployeeChart_Parser ep;
    ep.parse_preorder(root);
    ...
    MilitaryChart_Parser ep;
}


// Curiously recurring template pattern (static polymorphism, simulated polymorphism)



// Free Lunch??
/* No.
 * supposing we create another class MilitaryChart_Parser in the same way. Then Military and Employee Chart Parser will be
 * 2 distinguished classes that occupy its own space in the program image size.
*/


```
  
