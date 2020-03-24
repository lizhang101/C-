#include <iostream>
#include <thread>
#include <future>
#include <functional>
#include <deque>

using namespace std;
int factorial(int N) {
    int res = 1;
    for (int i = N; i > 1; i--) {
        res *= i;
    }
    cout << "thread: result is " << res << endl;
    return res;
}

deque<packaged_task<int()>> task_q;
mutex mu;
condition_variable cond;

void execute() {
    // KEY 1. be sure to lock first when ever access a shared data, which is the task_q!
    //        so we need to add the mutex.
    //     2. We also need condtion_variable to maintain the order !!

    packaged_task<int()> t;
    {
        unique_lock<mutex> locker(mu);
        cond.wait(locker, []() {return !task_q.empty(); });
        t = move(task_q.front());
        task_q.pop_front();
    }
    t();
}

int main() {
    ///// Create a package_task and call it later with a parameter. /////
    packaged_task<int(int)> t(factorial);
    t(6); // in another context
    int x;
    x = t.get_future().get(); // t() return type is void.
    cout << "main thread: x: " << x << endl;
    ////////////////////////////////////////////////////////////////////

    /////////// Using Bind ////////////////////////////////////////////////
    packaged_task<int()> t_bind(bind(factorial, 6));
    t_bind();
    x = t_bind.get_future().get();
    cout << "main thread, using bind: x: " << x << endl;
    //////////////////////////////////////////////////////////////////////

    //////////////////// Using Function

    thread t_execute(execute);

    auto tt = packaged_task<int()>(bind(factorial, 6));
    // since tt is not used anymore, we can move it into the other thread!
    future<int> fu = tt.get_future();
    task_q.push_back(move(tt));
    cond.notify_one();

    x =  fu.get();
    cout << "main thread, using task_q: x: " << x << endl;
    t_execute.join();
    return 0;
}
