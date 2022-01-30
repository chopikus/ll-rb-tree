#include <bits/stdc++.h>

using namespace std;

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

template<class ValueType>
class Set { 
    private:
        using Color = bool;
        static const Color BLACK = false;
        static const Color RED = true;

        class Node {
            Node(const ValueType& value): value(value), name(next_name()) {}
            Node* left = nullptr;
            Node* right = nullptr;
            Node* parent = nullptr;
            ValueType value;
            Color color = RED;
            int size = 1;
            friend class Set;
            string name="";
        };

    public:
        class Iterator {
            public: 
                const ValueType& operator*() const {
                    return node->value;             
                }
                
                const ValueType* operator->() const {
                    return &(node->value);
                }                

                bool operator!=(const Iterator& another) const {
                    return node != another.node;
                }
 
                bool operator==(const Iterator& another) const {
                    return node == another.node;
                }

                void operator++() {
                   node = set_->next_(node); 
                }

                void operator++(int) {
                    node = set_->next_(node);
                }
                
            private:
                Iterator(Node* node, const Set* set) : node(node), set_(set) {};
                Node* node = nullptr;
                const Set* set_;

            friend class Set;
        }; 
        
        Iterator begin() const {
            return Iterator(min_node(root), this);
        }

        Iterator end() const {
            return Iterator(nullptr, this);
        }

        bool empty() const {
            return (root == nullptr);
        }

        size_t size() const {
            if (root == nullptr) {
                return 0;
            } else {
                return root->size;
            }
        }

        void print_all() const {
            left_right(root);
        } 
 

        Set() {} 

        template<class ForwardIt>
            Set(ForwardIt first, ForwardIt last) {
                while (first != last) {
                    insert(*first);
                    first++;
                }
            } 

        Set(const std::initializer_list<ValueType> &list) {
            for (auto &element : list) {
                insert(element);
            }
        }

        void insert(const ValueType& value) {
            if (root == nullptr) {
                root = new Node(value);
            } else {
                root = insert(root, value);
            }
            root->color = BLACK;
            root->parent = nullptr;
        }

        void erase(const ValueType& value) {
            if (root == nullptr) {
                return;
            }
            if (get_color(root->left) == BLACK && get_color(root->right) == BLACK) {
                root->color = RED;
            }
            root = erase(root, value);
            if (root != nullptr) {
                root->color = BLACK;
                root->parent = nullptr;
            }
        }

        ~Set() {
            if (root != nullptr)
                clear(root);
            root = nullptr;
        } 

        string textify_color(Color color) {
            if (color == BLACK)
                return "BLACK";
            else
                return "RED";
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

        string visualise_wrapper() {
            Node* node = root;
            stringstream ss;
            ss << "digraph g {\n";
            ss << node->name << " [label=" << node->value << ", color = "<<textify_color(node->color) << "] \n";
            ss << visualize(node);
            ss << "}";
            return ss.str();
        }

    private:
        Node* root = nullptr; 

        Node* min_node(Node* node) const {
            if (node == nullptr)
                return nullptr;
            while (node->left != nullptr) {
                node = node->left;
            }
            return node;
        }

        Node* next_(Node* node) const {
            assert(node != nullptr);
            if (node->right) {
                return min_node(node->right);
            }
            while (node->parent) {
                if (node->parent->left == node) {
                    return node->parent;
                }
                node = node->parent;
            }
            return nullptr;
        }

        void left_right(Node* node) const {
            if (node==nullptr)
                return; 
            left_right(node->left);
            cout << node->value << ' ';
            left_right(node->right);
        }

        Color get_color(Node* node) const {
            if (node==nullptr)
                return BLACK;
            return node->color;
        }

        void recalc_size(Node* node) {
            node->size = 1;
            if (node->left)
                node->size += node->left->size;
            if (node->right)
                node->size += node->right->size;
        }

        void recalc_child_parent(Node* node) {
            if (node->left)
                node->left->parent = node;
            if (node->right)
                node->right->parent = node;
        }

        Node* rotate_left(Node* node) {
            assert(node->right != nullptr && node->right->color == RED);
            Node* r = node->right;
            node->right = r->left;
            r->left = node;

            recalc_size(r->left);
            recalc_size(r);

            recalc_child_parent(r->left);
            recalc_child_parent(r);

            r->color = r->left->color;
            r->left->color = RED;
            return r;
        }

        Node* rotate_right(Node* node) {
            assert(node->left != nullptr && node->left->color == RED);
            Node* l = node->left;
            node->left = l->right;
            l->right = node;

            recalc_size(l->right);
            recalc_size(l);

            recalc_child_parent(l->right);
            recalc_child_parent(l);

            l->color = l->right->color;
            l->right->color = RED;
            return l;
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
            recalc_size(node);
            recalc_child_parent(node);
            return node;
        }

        Node* insert(Node* node, const ValueType& value) {
            if (value == node->value)
                return balance(node);
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

        Node* erase_min(Node* node) {
            if (node->left == nullptr) {
                assert(node->right == nullptr);
                delete node;
                return nullptr; //deleting here
            }
            if (get_color(node->left) == BLACK && node->left != nullptr 
                    && get_color(node->left->left) == BLACK) {
                node = move_red_left(node);       
            }
            node->left = erase_min(node->left);
            return balance(node);
        }

        Node* erase(Node* node, const ValueType& value) {
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
                if (get_color(node->left) == RED) {
                    node = rotate_right(node);
                }
                if (!(node->value < value) && !(value < node->value) && node->right == nullptr) {
                    assert(node->left == nullptr);
                    delete node;
                    //deleting here
                    return nullptr;
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
                    node->right = erase_min(node->right); 
                }
            }
            return balance(node);
        }

        void clear(Node* root) {
            if (root->left)
                clear(root->left);
            if (root->right) 
                clear(root->right);
            delete root;
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
            cout << my_set.size() << endl;
        }
        if (x=='+') {
            int value = 0;
            cin >> value;
            cnt++;
            my_set.insert(value);
            //cout << tree->value << endl;
        } else if (x=='?') {
            for (auto s : my_set) {
                cout << s << ' ';
            }
            cout << endl;
        } else if (x == '-') {
            int value = 0;
            cin >> value;
            my_set.erase(value);
        } else if (x == 'e') {
            cout << (my_set.empty()) << endl;
        } else if (x == '!') {
            ofstream myfile;
            myfile.open ("output.dot");
            myfile << my_set.visualise_wrapper();
            myfile.close();
        }
    }
    //left_right(tree, 0);
    //cout << mx << endl;
    return 0;
}
