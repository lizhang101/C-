/*
 Multiprocessing:

 Multithreading:
 Pros:
 * fast to start
 * low overhead
 Cons:
 * difficult to implement
 * CAN"T RUN ON DISTRIBUTED SYSTEM
 */
#include <fstream>

#include <iostream>
#include <thread>
using namespace std;

#if 0
//////////Oversubscription//////////////
/* Don't run more threads than the HW concurrency */
int main() {
    auto max_t = thread::hardware_concurrency();
}
///////////////////////////////////////
#endif

#if 0
//////////Differentiating between threads/////////////
/* ---------------------
std::thread::get_id();
std::this_thread::get_id();
--------------------------*/
#include <thread>
void thread_function()
{
    std::cout<<"Inside Thread :: ID  = "<<std::this_thread::get_id()<<std::endl;
}
int main()
{
    std::thread threadObj1(thread_function);
    std::thread threadObj2(thread_function);

    if(threadObj1.get_id() != threadObj2.get_id()) //<<<KEY
        std::cout<<"Both Threads have different IDs"<<std::endl;

    std::cout<<"From Main Thread :: ID of Thread 1 = "<<threadObj1.get_id()<<std::endl;
    std::cout<<"From Main Thread :: ID of Thread 2 = "<<threadObj2.get_id()<<std::endl;

    threadObj1.join();
    threadObj2.join();
    return 0;
}
////////////////////////////////////////////////////
#endif

#if 0
//This example to show using a member function.
class Foo{
public:
    void boo(int x) {
        cout << "thread:" << x << endl;
    }
};
int main() {
    Foo foo_obj;
    thread t1(&Foo::boo, &foo_obj, 10); //<<KEY
    t1.join();
}


#endif

#if 0
// Example to use shared_ptr.
class Foo{

public:
    int data = 0;
    void operator() () {
        cout << "thread using shared_ptr:" << data << endl;
    }
};
int main() {
    auto foo_ptr = make_shared<Foo>();
    foo_ptr->data = 10;
    //This bind a member function with an object.
    thread t1(&Foo::operator(), foo_ptr); //<<<KEY
    t1.join();
}
#endif


#if 0
// Example passing by reference, move
class Fctor{
public:
    // Avoid passing by raw pointers or reference. The other thread has to make sure the shared data must be alive in this case.
    /* passing by value
    void operator()(string msg) {
        cout << "from t1:" << msg << endl;
        msg = "Trust is the mother of deceit.";//We want to change the msg
    }
    */
    // If you passing by reference, must use const. <<<KEY
    void operator()(const string &msg) {
        cout << "from t1:" << msg << endl;
        auto &msg_m = const_cast<string&>(msg);
        msg_m = "Trust is the mother of deceit.";//We want to change the msg
    }
};

int main() {
    string s = "Where there is no trust, there is no love.";
    //in this case, s will be copied into t1. so t1 won't be able to modify s even passing by reference.
    thread t1((Fctor()), s);// 1. note the (Factor()). 2. how to pass a parameter to Functor.
    t1.join();
    cout <<"Passing by value:" << s << endl;

    thread t2((Fctor()), ref(s)); //<<<KEY
    t2.join();
    cout <<"Passing by ref:" << s << endl;


    //passing by move. s will be empty after move
    s = "Where there is no trust, there is no love.";
    thread t3((Fctor()), move(s)); //<<<KEY
    t3.join();
    cout <<"Passing by ref:" << s << endl;

}
#endif


#if 0
// Example: Racing condition
// This example also showed passing by reference.
class Fctor {
    ofstream& m_str;
public:
    Fctor(ofstream& s):m_str(s) {}   // Reference member can only be initialized
    void operator()() {
        for (int i=0; i>-100; i--)
            //m_str << "from t1: " << i << endl;
            cout << "from t1:" << i << endl;
    }
};

int main() {
    cout << "Hollo Bo" << endl;
    ofstream f;
    f.open("log.txt");

    Fctor fctor(f);
    std::thread t1(fctor);

    for (int i=0; i<100; i++)
        //f << "from main: " << i << endl;
        cout << "from main: " << i << endl;

    t1.join();
    f.close();

    return 0;
}
#endif