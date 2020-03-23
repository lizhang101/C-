#include <iostream>
#include <deque>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>

using namespace std;
///////////// condition_variable ////////////////
/* 1. must use unique_lock since we are locking/unlocking multiple times.
 * 2. wait(locker): Unlock the locker first, lock again when condition triggered.
 *                  So we need to unlock it at the end.
 */
deque<int> queue;
mutex mu;
condition_variable cond;


void func_1() {
    for (int i = 0; i<10; i++) {
        unique_lock<mutex> locker(mu);
        queue.push_back(i);
        locker.unlock();
        cond.notify_one();
        //cond.notify_all();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << "thread1 exit" << endl;
}

void func_2() {
    int data = 0;
    int i = 0;
    //for (int i = 0; i<10; i++) {
    while(data != 9) {
        unique_lock<mutex> locker(mu);
        //KEY:  spurious wake
        //      The thread2 can wake up by itself. So we must check the condition.
        cond.wait(locker,[]() {return !queue.empty();} );
        data = queue.front();
        queue.pop_front();
        locker.unlock();
        cout << "t2 got :" << data  << "i: " << i << endl;
        i++;
    }
    cout << "thread2 exit" << endl;
}

int main() {
    thread t1(func_1);
    thread t2(func_2);
    t1.join();
    t2.join();
    return 0;
}
