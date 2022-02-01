#include <iostream>
#include <vector>
#include <algorithm>
#include "set.h"

using namespace std;

int main() {
    Set<pair<int, int> > my_set = {{1, 2}, {2,3}};
    auto it = my_set.begin();
    cout << (*it).first;
    return 0;
}
