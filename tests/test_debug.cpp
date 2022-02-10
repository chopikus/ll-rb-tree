#include <iostream>
#include <vector>
#include <algorithm>
#include "set.h"

using namespace std;

int main() {
    int q=0;
    cin >> q;
    int cnt=0;
    Set<int> my_set;
    for (int i=0; i<q; i++) {
        char x='1';
        cin >> x;
        if (x=='s') {
            cout << my_set.size() << endl;
        }
        if (x=='+') {
            int value = 0;
            cin >> value;
            cnt++;
            my_set.insert(value);
        } else if (x=='?') {
            vector<int> v;
            if (!my_set.empty()) {
                auto it = my_set.end();
                it--;
                while (it != my_set.begin()) {
                    v.push_back(*it);
                    it--;
                }
                v.push_back(*it);
            }
            reverse(v.begin(), v.end());
            for (auto s : v) {
                cout << s << ' ';
            }
            cout << endl;
        } else if (x == '-') {
            int value = 0;
            cin >> value;
            my_set.erase(value);
        } else if (x == 'x') {
            my_set.erase(my_set.begin());
        } else if (x == 'e') {
            cout << (my_set.empty()) << endl;
        } else if (x == '=') {
            int y=0;
            cin >> y;
            cout << (my_set.find(y) == my_set.end()) << endl;
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
