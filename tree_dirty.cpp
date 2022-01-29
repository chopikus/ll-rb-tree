#include <bits/stdc++.h>

using namespace std;

enum Color {
    RED,
    BLACK
};

string last_used_name="@";

string next_name() {
    string res = last_used_name;
    res.back()++;
    for (int i=res.size()-1; i>0; i--) {
        if (res[i]>'Z') {
            res[i] = 'A';
            res[i-1]++;
        }
    }
    if (res[0]>'Z') {
        res[0] = 'A';
        res = 'A'+res;
    }
    last_used_name = res;
    return res;
}

struct Node {
    Node* left = nullptr;
    Node* right = nullptr;
    int value = 0;
    Color color = RED;
    string name = "";
    Node(int value_): value(value_), name(next_name()) {}
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

Node* split_four_node(Node* node) {
    node = rotR(node);
    node->left->color = BLACK;
    return node;
}

Node* lean_left(Node* node) {
    node = rotL(node);
    node->color = node->left->color;
    node->left->color = RED;
    return node;
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
    //cout << node->value << ' ' << textify_color(get_color(node->left)) << ' ' << textify_color(get_color(node->right)) << endl;
    if (get_color(node->left)==BLACK && get_color(node->right)==RED) {
        node = lean_left(node);
    }
    if (get_color(node->left)==RED && get_color(node->left->left)==RED) {
        node = split_four_node(node);
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

Node* move_red_left(Node* node) {
    node->color = BLACK;
    node->left->color = RED;
    if (node->right != nullptr) {
        if (get_color(node->right->left)==RED) {
            node->right = rotR(node->right);
            node = rotL(node);
        } else {
            node->right->color = RED;
        }
    }
    return node;
}

Node* move_red_right(Node* node) {
    node->color = BLACK;
    node->right->color = RED;
    if (node->left != nullptr && get_color(node->left->left)==RED) {
        node = rotR(node);
        node->color = RED;
        node->left->color = BLACK;
    } else {
        node->left->color = RED;
    }
    return node;
}

Node* delete_min(Node* node) {
    if (node->left == nullptr) {
        assert(node->right == nullptr);
        return nullptr;
    }
    if (get_color(node->left) == BLACK && node->left != nullptr 
            && get_color(node->left->left) == BLACK) {
        node = move_red_left(node);       
    }
    node->left = delete_min(node->left);
    if (get_color(node->left)==BLACK && get_color(node->right)==RED) {
        node = lean_left(node);
    }
    if (get_color(node->left)==RED && get_color(node->right)==RED) {
        node = blackify_color(node);
    }
    return node;
}

Node* delete_min_wrapper(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    node = delete_min(node);
    if (node != nullptr)
        node->color = BLACK;
    return node;
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

string visualize(Node* node) {
    if (node==nullptr) {
        return "";
    }
    stringstream ss;
    if (node->left != nullptr) {
        Node* l = node->left;
        ss << l->name << " [label=" << l->value << ", color = " << textify_color(l->color) << "] \n";
        ss << node->name << "-> " << l->name << endl;
        ss << visualize(l);
    }
    if (node->right != nullptr) {
        Node* r = node->right;
        ss << r->name << " [label=" << r->value << ", color = " << textify_color(r->color) << "] \n";
        ss << node->name << "-> " << r->name << endl;
        ss << visualize(r);
    }
    return ss.str();
}

string visualise_wrapper(Node* node) {
    stringstream ss;
    ss << "digraph g {\n";
    ss << node->name << " [label=" << node->value << ", color = "<<textify_color(node->color) << "] \n";
    ss << visualize(node);
    ss << "}";
    return ss.str();
}

int main() {
    int q=0;
    cin >> q;
    Node* tree = nullptr;
    int cnt=0;
    for (int i=0; i<q; i++) {
    //while (true) {
        char x='1';
        cin >> x;
        if (x=='s') {
            break;
        }
        if (x=='+') {
            int value = 0;
            cin >> value;
            cnt++;
            tree = insert_wrapper(tree, value);
            //cout << tree->value << endl;
        } else if (x=='?') {
            left_right(tree, 0);
            cout << endl;
        } else if (x == '!') {
            ofstream myfile;
            myfile.open ("output.dot");
            myfile << visualise_wrapper(tree);
            myfile.close();
        } else if (x == '-') {
            tree = delete_min_wrapper(tree); 
        }
    }
    return 0;
}
