#include <bits/stdc++.h>

using namespace std;

template<class ValueType>
class Set { 
    public:
        Set() {} 
        void insert(ValueType value) {
            if (root == nullptr) {
                root = new Node(value);
            } else {
                root = insert(root, value);
            }
            root->color = BLACK;
        }
        void erase(ValueType value) {
            if (root == nullptr) {
                return;
            }
            if (get_color(root->left) == BLACK && get_color(root->right) == BLACK) {
                root->color = RED;
            }
            root = erase(root, value);
            if (root != nullptr)
                root->color = BLACK;
        }
        void print_all() {
            left_right(root);
        }
    private:
        using Color = bool;
        const Color BLACK = false;
        const Color RED = true;

        class Node {
            public:
                Node(int value): value(value) {}
                Node* left = nullptr;
                Node* right = nullptr;
                ValueType value = 0;
                Color color = RED;
        };
        Node* root = nullptr;

        void left_right(Node* node) {
            if (node==nullptr)
                return; 
            left_right(node->left);
            cout << node->value << ' ';
            left_right(node->right);
        }

        Color get_color(Node* node) {
            if (node==nullptr)
                return BLACK;
            return node->color;
        }

        Node* rotate_left(Node* node) {
            assert(node->right != nullptr && node->right->color == RED);
            Node* r = node->right;
            node->right = r->left;
            r->left = node;
            node = r;
            node->color = node->left->color;
            node->left->color = RED;
            return node;
        }

        Node* rotate_right(Node* node) {
            assert(node->left != nullptr && node->left->color == RED);
            Node* l = node->left;
            node->left = l->right;
            l->right = node;
            node = l;
            node->color = node->right->color;
            node->right->color = RED;
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
                delete node;
                return nullptr; //deleting here
            }
            if (get_color(node->left) == BLACK && node->left != nullptr 
                    && get_color(node->left->left) == BLACK) {
                node = move_red_left(node);       
            }
            node->left = delete_min(node->left);
            return balance(node);
        }

        Node* min_node(Node* node) {
            if (node == nullptr)
                return nullptr;
            while (node->left != nullptr) {
                node = node->left;
            }
            return node;
        }

        Node* erase(Node* node, int value) {
            if (node == nullptr) {
                return nullptr;
            }
            if (value < node->value) {
                if (get_color(node->left) == BLACK && node->left != nullptr 
                        && get_color(node->left->left) == BLACK) {
                    node = move_red_left(node);
                }
                node->left = erase(node->left, value);
            } else {
                if (!(node->value < value) && node->right == nullptr) {
                    assert(node->left == nullptr);      
                    delete node;
                    //deleting here
                    return nullptr;
                }
                if (get_color(node->left) == RED) {
                    node = rotate_right(node);
                } 
                if (get_color(node->right) == BLACK && node->right != nullptr 
                        && get_color(node->right->left) == BLACK) {
                    node = move_red_right(node);
                }
                if (node->value < value) {
                    node->right = erase(node->right, value);
                } else {
                    assert(node->right != nullptr);
                    Node* min_right_node = min_node(node->right);
                    node->value = min_right_node->value;
                    node->right = delete_min(node->right); 
                }
            }
            return balance(node);
        }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int q=0;
    cin >> q;
    int cnt=0;
    Set<int> my_set;
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
            my_set.insert(value);
            //cout << tree->value << endl;
        } else if (x=='?') {
            my_set.print_all();
            cout << endl;
        } else if (x == '-') {
            int value = 0;
            cin >> value;
            my_set.erase(value);
        }
    }
    //left_right(tree, 0);
    //cout << mx << endl;
    return 0;
    }
