#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n=0;
    cin >> n;
    set<int> Set;
    for (int i=0; i<n; i++) {
        char x='1';
        cin >> x;
        if (x=='+') {
            int y=0;
            cin >> y;
            Set.insert(y);
        }
        if (x=='?') {
            for (auto s : Set) {
                cout << s << ' ';
            }
            cout << endl;
        }
        if (x=='-') {
            int y=0;
            cin >> y;
            Set.erase(y);
        }
        if (x=='s') {
            cout << Set.size() << endl;
        }
        if (x=='e') {
            cout << Set.empty() << endl;
        }
    }
    return 0;
}
