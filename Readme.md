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
  
* Use composition whenever possible, use priviate inheritance when have to. Why?
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
  
  
