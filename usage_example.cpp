#include <algorithm>
#include <iostream>
#include <vector>
#include "src/set.h"

using namespace std;

int main() {
    int q=0;
    cin >> q;
    int cnt=0;
    llrb::Set<int> my_set;
    for (int i=0; i<q; i++) {
        char x='1';
        cin >> x;
        if (x=='+') {
            int value = 0;
            cin >> value;
            cnt++;
            my_set.insert(value);
        } else if (x=='?') {
            for (auto s : my_set) {
                cout << s << ' ';
            }
            cout << endl;
        } else if (x == '-') {
            int value = 0;
            cin >> value;
            my_set.erase(value);
        } else if (x == '>') {
            int y=0;
            cin >> y;
            if (my_set.lower_bound(y) != my_set.end()) {
                cout << *(my_set.lower_bound(y)) << endl;
            }
        }
    }
    return 0;
}