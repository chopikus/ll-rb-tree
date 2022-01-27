#include <bits/stdc++.h>

using namespace std;

enum Color {
    RED,
    BLACK
};

struct Node {
    Node* left = nullptr;
    Node* right = nullptr;
    int value = 0;
    Color color = RED;
    Node(int value_): value(value_) {}
};

string textify_color(Color color) {
    if (color == BLACK)
        return "BLACK";
    else
        return "RED";
}

Color get_color(Node* node) {
    if (node==nullptr)
        return BLACK;
    return node->color;
}

Node* rotL(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

Node* rotR(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    return x;
}

Node* blackify_color(Node* node) { //should be called only if colors of both sons are RED
    node->color = RED;
    node->left->color = BLACK;
    node->right->color = BLACK;
    return node;
}

Node* insert(Node* node, int value) {
    int node_value = node->value;
    if (value == node->value)
        return node;
    if (value > node->value) {
        if (node->right == nullptr) {
            node->right = new Node(value);
        } else {
            node->right = insert(node->right, value);
        }
    }
    if (value < node->value) {
        if (node->left == nullptr) {
            node->left = new Node(value);
        } else {
            node->left = insert(node->left, value);
        }
    }
    if (get_color(node->left)==BLACK && get_color(node->right)==RED) {
        node = rotL(node);
        node->left->color = RED;
        node->color = BLACK;
    }
    if (get_color(node->left)==RED && get_color(node->left->left)==RED) {
        node = rotR(node);
        node->left->color = BLACK;
    }
    if (get_color(node->left)==RED && get_color(node->right)==RED) {
        node = blackify_color(node);        
    }
    return node;
}

Node* insert_wrapper(Node* node, int value) {
    Node* return_node = nullptr;
    if (node == nullptr) {
        return_node = new Node(value);
    } else {
        return_node = insert(node, value);
    }
    return_node->color = BLACK;
    return return_node;
}

int mx=0;

void left_right(Node* node, int h) {
    if (node==nullptr)
        return; 
    left_right(node->left, h+1);
    mx = max(mx, h);
    cout << node->value << ' ';
    left_right(node->right, h+1);
}

int main() {
    int q=0;
    cin >> q;
    Node* tree = nullptr;
    int cnt=0;
    for (int i=0; i<q; i++) {
        char x='1';
        cin >> x;
        if (x=='+') {
            int value = 0;
            cin >> value;
            cnt++;
            tree = insert_wrapper(tree, value);
            //cout << tree->value << endl;
        } else if (x == '?') {
            left_right(tree, 0);        
            cout << endl;
        } else if (x == '-') {
                
        }
    }
    return 0;
}
