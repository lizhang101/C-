//################################
/*
 Issues in following code:
 the d.seeCat will finally call the dog::bark(), even though it's a yellowdog.
 the fix is to declare bark() as virtual function.
 */

class dog {
public:
    string m_name;
    dog(string name) {m_name = name;  bark();}
    void bark() { cout<< "Woof, I am just a dog " << m_name << endl;}
    void seeCat() { bark(); }
};

class yellowdog : public dog {
public:
    yellowdog(string name) : dog(string name) {...}
    void bark() { cout << "Woof, I am a yellow dog " << m_name << endl; }
};

int main ()
{
    yellowdog d;
    d.seeCat();

}


//############################################################################
/*
Never call virtual functions in constructor or destructor.
*/
class dog {
public:
    string m_name;
    dog(string name) {m_name = name;  bark();}
    virtual void bark() { cout<< "Woof, I am just a dog " << m_name << endl;}
};

class yellowdog : public dog {
public:
    yellowdog(string name) : dog(string name) {...}
    virtual void bark() { cout << "Woof, I am a yellow dog " << m_name << endl; }
};

int main ()
{
    yellowdog mydog("Bob");
}



OUTPUT:
Woof, I am just a dog Bob.

/*

During the construction, all virtual function works like non-virtual function.

Why?
Base class's constructor run before derived class's constructor. So at the
time of bark(), the yellowlog is not constructed yet.


Why Java behaves differently?

There is a fundamental difference in how Java and C++ defines an object's Life time.
Java: All members are null initialized before constructor runs. Life starts before constructor.
C++: Constructor is supposed to initialize members. Life starts after constructor is finished.

Calling down to parts of an object that haven not yet initialized is inherently dangerous.

*/


/*
solution 1:
*/
class dog {
public:
    ...
    dog(string name, string color) {m_name = name; bark(color);}
    void bark(string str) { cout<< "Woof, I am "<< str << " dog " << m_name << endl;}
};

class yellowdog : public dog {
public:
    yellowdog(string name):dog(name, "yellow") {}
};

int main ()
{
    yellowdog mydog("Bob");
}

OUTPUT:
Woof, I am yellow dog Bob



/*
solution 2:
*/
class dog {
public:
    ...
    dog(string name, string woof) {m_name = name; bark(woof);}
    dog(string name) {m_name = name; bark( getMyColor() );}
    void bark(string str) { cout<< "Woof, I am "<< str << "	private:
        private:
        static string getMyColor() {return "just a";}
    };

    class yellowdog : public dog {
    public:
        yellowdog(string name):dog(name, getMyColor()) {}
    private:
        static string getMyColor() {return "yellow";}  //Why static?
    };

int main ()
{
    yellowdog mydog("Bob");
}

OUTPUT:
Woof, I am yellow dog Bob