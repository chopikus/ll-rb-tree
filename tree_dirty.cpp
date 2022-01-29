#include <bits/stdc++.h>

using namespace std;

using Color = bool;

string last_used_name="@";
const Color BLACK = false;
const Color RED = true;

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

string textify_color(Color color);
Color get_color(Node* node);
Node* rotL(Node* x);
Node* rotR(Node* y);
Node* rotate_left(Node* node);
Node* rotate_right(Node* node);
Node* flip_colors(Node* node);
Node* balance(Node* node);
Node* insert(Node* node, int value);
Node* insert_wrapper(Node* node, int value);
Node* move_red_left(Node* node);
Node* move_red_right(Node* node);
Node* delete_min(Node* node);
Node* delete_min_wrapper(Node* node);
Node* min_node(Node* node);
Node* remove(Node* node, int value);
Node* remove_wrapper(Node* node, int value);
void left_right(Node* node, int h);
string visualize(Node* node);
string visualise_wrapper(Node* node);

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

Node* rotate_right(Node* node) {
    assert(node->left != nullptr && node->left->color == RED);
    node = rotR(node);
    node->color = node->right->color;
    node->right->color = RED;
    return node;
}

Node* rotate_left(Node* node) {
    assert(node->right != nullptr && node->right->color == RED);
    node = rotL(node);
    node->color = node->left->color;
    node->left->color = RED;
    return node;
}

Node* flip_colors(Node* node) {
    assert(node != nullptr && node->left != nullptr && node->right != nullptr && node->left->color != node->color && node->right->color != node->color);
    node->color = !node->color;
    node->left->color = !node->left->color;
    node->right->color = !node->right->color;
    return node;
}

Node* balance(Node* node) {
    if (get_color(node->left)==BLACK && get_color(node->right)==RED) {
        node = rotate_left(node);
    }
    if (get_color(node->left)==RED && get_color(node->left->left)==RED) {
        node = rotate_right(node);
    }
    if (get_color(node->left)==RED && get_color(node->right)==RED) {
        node = flip_colors(node);
    }   
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
    return balance(node);
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
    flip_colors(node);
    if (node->right != nullptr && get_color(node->right->left) == RED) {
        node->right = rotate_right(node->right);
        node = rotate_left(node);
        flip_colors(node);
    }
    return node;
}

Node* move_red_right(Node* node) {
    flip_colors(node);
    if (node->left != nullptr && get_color(node->left->left) == RED) {
        node = rotate_right(node);
        flip_colors(node);
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
    return balance(node);
}

Node* delete_min_wrapper(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    if (get_color(node->left) != RED && get_color(node->right) != RED) {
        node->color = RED;
    }
    node = delete_min(node);
    if (node != nullptr)
        node->color = BLACK;
    return node;
}

Node* min_node(Node* node) {
    if (node == nullptr)
        return nullptr;
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

Node* remove(Node* node, int value) {
    if (node == nullptr) {
        return nullptr;
    }
    if (value < node->value) {
        if (get_color(node->left) == BLACK && node->left != nullptr 
                && get_color(node->left->left) == BLACK) {
            node = move_red_left(node);
        }
        node->left = remove(node->left, value);
    } else {
        if (get_color(node->left) == RED) {
            node = rotate_right(node);
        }
        if (node->value == value && node->right == nullptr) {
            assert(node->left == nullptr); //why??
            return nullptr;
        }
        if (get_color(node->right) == BLACK && node->right != nullptr 
                && get_color(node->right->left) == BLACK) {
            node = move_red_right(node);
        }
        if (node->value == value) {
            assert(node->right != nullptr);
            Node* min_right_node = min_node(node->right);
            node->value = min_right_node->value;
            node->right = delete_min(node->right); 
        } else {
            node->right = remove(node->right, value);
        }
    }
    return balance(node);
}

Node* remove_wrapper(Node* node, int value) {
    if (node == nullptr) {
        return nullptr;
    }
    if (get_color(node->left) == BLACK && get_color(node->right) == BLACK) {
        node->color = RED;
    }
    node = remove(node, value);
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
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
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
            //left_right(tree, 0);
            //cout << endl;
        } else if (x == '!') {
            ofstream myfile;
            myfile.open ("output.dot");
            myfile << visualise_wrapper(tree);
            myfile.close();
        } else if (x == 'x') {
            tree = delete_min_wrapper(tree); 
        } else if (x == '-') {
            int value = 0;
            cin >> value;
            tree = remove_wrapper(tree, value);
        }
    }
    //left_right(tree, 0);
    //cout << mx << endl;
    return 0;
}
