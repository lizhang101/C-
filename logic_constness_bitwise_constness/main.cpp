#include <iostream>
#include <vector>

using namespace std;
class BigArray {
    vector<int> v;
    //this variable is changed in an const function, so it has to be "mutable"
    // this is to solve the conflict of logical & bitwise constness.
    // the getItem() is a logic const function while not a bitwise const.
    mutable int accessCounter;

    int *v2;
public:
    int getItem(int index) const {
        accessCounter++;
        return v[index];
    }

    //this function is a bitwise const function since it didn't change members
    // but it's not a logic const function. So the const has no effect on this.
    void setV2Item(int index, int x) const {
        *(v2 + index) = x;
    }

};

int main() {
    BigArray b;

    return 0;
}
