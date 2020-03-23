#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
//////////////MUTEX//////////
/* lock, unlock, and deadlock */
#if 0
//problem 2: the global mu can be accessed without going through the lock
mutex mu;
void shared_print_v0(string msg, int id) {
    //KEY:
    //1. each print is guarded, but the prints from different threads can interleave in line level.
    mu.lock();
    cout << msg << id << endl; //<<<KEY: problem 1:If this line throw exception,
                               // mu won't be unlocked, and cause a deadlock!
    mu.unlock();
}
void shared_print_v1(string msg, int id) {
    lock_guard<mutex> guard(mu); // <<<KEY: to solve problem 1.
    //mu.lock();
    cout << msg << id << endl;
    //mu.unlock();
}

void func() {
    for (int i = -1; i>-11; i--) {
        shared_print(string("From t1: "), i);
    }
}

int main() {
    thread t1(func);
    for (int i = 0; i<10; i++) {
        shared_print(string("From main: "), i);
    }
    t1.join();
    return 0;
}
#endif
///////////////////////////////////

////////////Continue to show how to solve problem 2//////////////
#if 0
#include <fstream>
class LogFile {
    mutex mu_;
    ofstream f;
public:
    LogFile() {
        f.open("log.txt");
    }
    void share_print(string msg, int value) {
        lock_guard<mutex> guard(mu_);
        f << "From " << msg << ": " << value << endl;
    }

    //KEY Never return f to outside world!!
    ofstream & getStream() { return f; }

    // KEY Never pass f as an argument to user provided function:
    void processf(void fun(ofstream&)) {
        fun(f);
    }
};
void thread_func(LogFile &log) {
    for (int i=0; i>-10; i--) {
        log.share_print(string("From t1: "), i);
    }
}
int main() {
    LogFile log;
    thread t1(thread_func, ref(log));
    for (int i=0; i<10; i++) {
        log.share_print(string("From main: "), i);
    }
    t1.join();

}
#endif
////////////////////////////


/////////////STL///////////////
#if 0
class stack {
    int *data_;
    mutex mu_;
public:
    void pop(){};
    int& top(){};
};
void process(int v) {}
void func1(stack& st) {
    int v = st.top();
    st.pop();
    process(v);
}
/*Question: is this stack thread safe?
 * Let consider the case:
 * stack st[9,3,8,6|
 * threadA                          threadB
 * int v = st.top(); //6
 *                                  int v = st.top(); //6
 * st.pop(); //[9,3,8|
 *                                  st.pop() //[9,3| 8 is discarded w/o process!!
 * process(v);
 *                                  process(v); //6 is processed twice!!
 * the fix is, pop return and pop the top of the stack.
 */
class stack_safe {
    int *data_;
    mutex mu_;
public:
    int pop(){}; //KEY: this function may not be exception safe!!
    int& top(){};
};
void func2(stack& st) {
    int v = st.pop();
    process(v);
}

#endif

///////////////////////////////


//////////////Dead Lock///////////////
#if 0
class LogFile {
    mutex mu_;
    mutex mu2_;
public:
    LogFile() { }
    void share_print(string msg, int value) {
        lock_guard<mutex> locker(mu_);
        lock_guard<mutex> locker2(mu2_);
        cout << "From " << msg << ": " << value << endl;
    }
    void share_print2(string msg, int value) {
        //KEY: the order is different, ABBA lock!
        lock_guard<mutex> locker2(mu2_);
        lock_guard<mutex> locker1(mu_);
        cout << "From " << msg << ": " << value << endl;
    }
};
void thread_func(LogFile &log) {
    for (int i=0; i>-100; i--) {
        log.share_print(string("From t1: "), i);
    }
}
int main() {
    LogFile log;
    thread t1(thread_func, ref(log));
    for (int i=0; i<100; i++) {
        log.share_print2(string("From main: "), i);
    }
    t1.join();
}
#endif
//FIX:
// 1. make sure lock in the same order: ABAB
// 2. use std::lock()
//////////////////////////////////////

///////////CONTINUE THE FIX///////////
#if 0
class LogFile {
    mutex mu_;
    mutex mu2_;
public:
    LogFile() { }
    void share_print(string msg, int value) {
        //Lock both first.
        lock(mu_, mu2_); //<<<KEY
        lock_guard<mutex> locker(mu_, adopt_lock);
        lock_guard<mutex> locker2(mu2_, adopt_lock);
        cout << "From " << msg << ": " << value << endl;
    }
    void share_print2(string msg, int value) {
        //KEY: the order is different, ABBA lock!
        lock(mu_, mu2_); //<<<KEY
        lock_guard<mutex> locker2(mu2_, adopt_lock);
        lock_guard<mutex> locker(mu_, adopt_lock);
        cout << "From " << msg << ": " << value << endl;
    }

    void share_print3(string msg, int value) {
        //Avoid to use 2 lockers at the same time. one for each operation.
        {
            lock_guard<mutex> locker(mu_);
            //do something....
        }
        {
            lock_guard<mutex> locker2(mu2_);
            //something else....
            cout << "From " << msg << ": " << value << endl;
        }
    }

};
void thread_func(LogFile &log) {
    for (int i=0; i>-100; i--) {
        log.share_print(string("From t1: "), i);
    }
}
int main() {
    LogFile log;
    thread t1(thread_func, ref(log));
    for (int i=0; i<100; i++) {
        log.share_print2(string("From main: "), i);
    }
    t1.join();
}
#endif
/* 1. Prefer locking a single mutex at a time. see share_print3
 * 2. avoid locking a mutex and then call a user provided function.(avoid that function lock the same mutex, or lock
 * multiple mutexes.
 * 3. use std::lock() to lock more than one mutex at a time.
 * 4. lock the mutex in the same order.
 * 5. hierarchical mutex. lower level can't lock high level.
 *
 * Locking granularity
 * Fine-grained: easy to have deadlock
 * Coarse-grained: lose parallel opportunities.
 */
/////////////////////////////////

//////////// Unique Mutex and Lazy Initialization /////////////////////
#if 0
class LogFile {
    mutex mu_;
public:
    LogFile() { }
    void share_print(string msg, int value) {
        //KEY : we only want to protect part of the execution.
        unique_lock<mutex> locker(mu_);
        //unique_lock<mutex> locker(mu_, defer_lock); //<<<< KEY: you can defer the lock, just locker.lock() later.
        cout << "From " << msg << ": " << value << endl;
        locker.unlock();
        //continue do something else that doesn't need to lock protection.
        locker.lock(); // Entering another critical section.

        //KEY: a unique_lock can only be moved.
        unique_lock<mutex> locker2 = move(locker);
    }
};
void thread_func(LogFile &log) {
    for (int i=0; i>-10; i--) {
        log.share_print(string("From t1: "), i);
    }
}
int main() {
    LogFile log;
    thread t1(thread_func, ref(log));
    for (int i=0; i<10; i++) {
        log.share_print(string("From main: "), i);
    }
    t1.join();
}
#endif


///////////////////////////////////////////////////////////////////////

class LogFile {
    mutex mu_;
    mutex mu_open_;
    once_flag flag_;
    ofstream f_;
public:
    LogFile() { }
    void share_print1(string msg, int value) {
        //KEY problem of this code: the file may be opened twice because is_open() is not protected.
        // See share_print2 for fix.
        if (!f_.is_open()) {
            unique_lock<mutex> locker (mu_open_);
            f_.open("log.txt");
        }
    }
    void share_print2(string msg, int value) {
        {
            //KEY: now it's thread safe. But every time we print, this part of code will be executed.
            //     See print3.
            unique_lock<mutex> locker(mu_open_);
            if (!f_.is_open()) {
                f_.open("log.txt");
            }
        }
    }

    void share_print3(string msg, int value) {
            //KEY:
         call_once(flag_, [&]() {f_.open("log.txt"); });
    }
};
void thread_func(LogFile &log) {
    for (int i=0; i>-10; i--) {
        log.share_print(string("From t1: "), i);
    }
}
int main() {
    LogFile log;
    thread t1(thread_func, ref(log));
    for (int i=0; i<10; i++) {
        log.share_print(string("From main: "), i);
    }
    t1.join();
}
