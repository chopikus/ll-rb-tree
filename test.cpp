#include <iostream>
#include <vector>
#include <algorithm>
#include "set.h"

using namespace std;

int main() {
    Set<int> my_set, my_set2, my_set3;
    my_set.insert(5);
    auto it = my_set.find(5);
    auto it2 = it;
    my_set3 = my_set2 = my_set;
    cout << (*it2) << endl;
    return 0;
}
