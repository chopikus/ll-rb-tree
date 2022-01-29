#include "testlib.h"
#include <bits/stdc++.h>

using namespace std;

const int N = 50000;

int main(int argc, char* argv[]) {
    registerGen(argc, argv);
    cout << N << endl;
    vector<int> v;
    for (int i=0; i<N; i++) {
        int t = rnd.next(0, 100);
        if (t <= 3) {
            cout << "?" << endl;
        } else if (t <= 67) {
            int num = rnd.next(-1000000000, 1000000000);
            v.push_back(num);
            cout << "+ " << num << endl;
        } else {
            cout << "- " << endl;
            /*if (rnd.next(1, 10) <= 7 && v.size()>0)
                cout << "- " << v[rnd.next(v.size())] << endl;
            else
                cout << "- " << rnd.next(-10000, 10000) << endl;*/
        }
    }
    return 0;
}
