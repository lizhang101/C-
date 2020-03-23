#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <functional>

using namespace std;
once_flag flag;

class A {
public:
    void f(int x, char c) { }
    int operator()(int N) { return N; }
};
void foo(int x) { }
int main() {
    A a;
    thread t1(a, 6); // functor a(), using copy of a in t1.
    thread t2(ref(a), 6); //
    thread t3(move(a), 6); //
    thread t4(A(), 6); // temp a is moved into t4
    thread t5([](int x) { return x * x; }, 6); //
    thread t6(foo, 7);
    thread t7(&A::f, a, 8, 'w'); // copy of a, a.f(8, 'w') is called in t7.
    thread t8(&A::f, &a, 8, 'w'); // a.f(8, 'w') is called in t8.
    async(std::launch::async, a, 6);
    bind(a, 6);
    call_once(flag, a, 6);

    // use the same formats for bind, async.


    return 0;
}
