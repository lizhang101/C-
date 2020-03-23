#include <iostream>
#include <thread>
#include <chrono>
#include <future>
using namespace std;

int factorial(int N) {
    int res = 1;
    for (int i = N; i > 1; i--) {
        res *= i;
    }
    return res;
}

int factorial_1(future<int> &f) {
    int res = 1;
    // if promise is broken, this function will throw an exception:
    // std::future_errc::broken_promise
    int N = f.get();

    for (int i = N; i > 1; i--) {
        res *= i;
    }
    cout << "thread result:" << res << endl;
    return res;
}
int factorial_2(const shared_future<int> &f) {
    int res = 1;
    // if promise is broken, this function will throw an exception:
    // std::future_errc::broken_promise
    int N = f.get();

    for (int i = N; i > 1; i--) {
        res *= i;
    }
    cout << "thread result:" << res << endl;
    return res;
}

int main() {
    int x;
    future<int> fu = async(factorial, 4);

    // the factorial will be called with the same thread when get is called!
    //future<int> fu = async(launch::deferred, factorial, 4);

    //the factorial will be surely called in another thread.
    //future<int> fu = async(launch::async, factorial, 4);

    //the factorial will be called in another thread or not, depending on the impl. this is the default setting.
    //future<int> fu = async(launch::async|launch::deferred, factorial, 4);

    x = fu.get();

    //Don't try to call get again. will crash.
    //fu.get()

    cout << "factorial of 4: " << x << endl;

    /* promise */
    promise<int> p;
    future<int> f_p = p.get_future();
    future<int> fu2 = async(launch::async, factorial_1, ref(f_p));
    //You can only move a future, promise
    //future<int> fu_t = move(fu2);
    // do something else...
    this_thread::sleep_for(chrono::milliseconds(100));
    p.set_value(4);

    //if you really can't keep promise, you can set an exception.
    //p.set_exception(make_exception_ptr(runtime_error("To err is human")));

    x = fu2.get();
    cout << "promise factorial of 4: " << x << endl;

    /*
     * launch many threads: all the threads share the same future. Note the future can only be moved.
     * So we can use the shared_future.
     * Broadcast the value to multiple threads.
     * note we also need to change the factorial().
     */
    promise<int> p1;
    future<int> f_p2 = p1.get_future();
    shared_future<int> sf = f_p2.share(); //<<< KEY
    future<int> fu_0 = async(launch::async, factorial_2, sf); // shared_future can be copied.
    future<int> fu_1 = async(launch::async, factorial_2, sf);
    future<int> fu_2 = async(launch::async, factorial_2, sf);
    //.... more threads...
    p1.set_value(4);
    x = fu_0.get();
    cout << "shared_future promise factorial of 4: " << x << endl;
    return 0;
}
