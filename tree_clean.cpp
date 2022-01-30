#include <bits/stdc++.h>

using namespace std;

template<class ValueType>
class Set { 
    private:
        using Color = bool;
        static const Color BLACK = false;
        static const Color RED = true;

        class Node {
            Node* left = nullptr;
            Node* right = nullptr;
            Node* parent = nullptr;
            ValueType value;
            Color color = RED;
            int size = 1;
            friend class Set;

            Node() {}

            Node(const ValueType& value): value(value) {}
            Node(const Node& another) {
                *this = another;
            }
    
            Node& operator=(const Node& another) {
                value = another.value;
                color = another.color;
                size = another.size;
                return *this;
            }
            
            void recalc_size() {
                size = 1;
                if (left)
                    size += left->size;
                if (right)
                    size += right->size;
            }

           void recalc_parents() {
                parent = nullptr;
                if (left)
                    left->parent = this;
                if (right)
                    right->parent = this;
            }

            void copy(const Node& another) {
                *this = another;
                delete left;
                delete right;
                if (another.left)
                    left = new Node();
                if (another.right)
                    right = new Node();

                if (left)
                    left->copy(*(another.left));
                if (right)
                    right->copy(*(another.right));
                this->recalc_parents();
            }

            void clear() {
                if (this->left)
                    this->left->clear();
                if (this->right) 
                    this->right->clear();
                delete this;
            }
        };
         
    public:
        class Iterator {
            public: 
                Iterator(const Iterator& another) = default;
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
                   node = set_->next(node); 
                }

                void operator++(int) {
                    node = set_->next(node);
                }
                
                void operator--() {
                    node = set_->prev(node);
                }
                
                void operator--(int) {
                    node = set_->prev(node);
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
        
        Iterator find(const ValueType& value) const {
            return Iterator(find(root, value), this);
        }
        
        Iterator lower_bound(const ValueType& value) const {
            return Iterator(lower_bound(root, value), this);
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
        
        Set(const Set& another) {
            *this = another;     
        }
    
        Set& operator=(const Set& another) {
            if (root)
                root->clear();
            if (another.root) {
                root = new Node();
                root->copy(*(another.root));
            }
            return *this;
        }

        void insert(const ValueType& value) {
            if (root == nullptr) {
                root = new Node(value);
            } else {
                root = insert(root, value);
            }
            root->color = BLACK;
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
            }
        }

        void erase(const Iterator& iterator) {
            if (iterator.node != nullptr) {
                erase(*iterator);
            }
        }

        ~Set() {
            if (root)
                root->clear();
            root = nullptr;
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

        Node* max_node(Node* node) const {
            if (node == nullptr)
                return nullptr;
            while (node->right != nullptr) {
                node = node->right;
            }
            return node;
        }
        
        Node* prev(Node* node) const {
            if (node == nullptr) {
                return max_node(root);
            }
            if (node -> left) {
                return max_node(node->left);
            }
            while (node -> parent) {
                if (node -> parent -> right == node) {
                    return node->parent;
                }
                node = node->parent;
            }
            return nullptr;
        }

        Node* next(Node* node) const {
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

        Color get_color(Node* node) const {
            if (node==nullptr)
                return BLACK;
            return node->color;
        }
            
        Node* find(Node* node, const ValueType& value) const {
            if (node == nullptr) {
                return nullptr;
            }
            if (value < node->value) {
                return find(node->left, value);
            } else if (node->value < value) {
                return find(node->right, value);
            } else {
                return node;
            }
        }

        Node* lower_bound(Node* node, const ValueType& value) const {
            if (node == nullptr) {
                return nullptr;
            }
            if (node->left && (!(node->left->value < value))) {
                return lower_bound(node->left, value);
            } else if (!(node->value < value)) {
                return node;
            } else {
                return lower_bound(node->right, value); 
            }
       } 

        Node* rotate_left(Node* node) {
            Node* r = node->right;
            node->right = r->left;
            r->left = node;

            r->left->recalc_size();
            r->recalc_size();

            r->left->recalc_parents();
            r->recalc_parents();

            r->color = r->left->color;
            r->left->color = RED;
            return r;
        }

        Node* rotate_right(Node* node) {
            Node* l = node->left;
            node->left = l->right;
            l->right = node;

            l->right->recalc_size();
            l->recalc_size();

            l->right->recalc_parents();
            l->recalc_parents();

            l->color = l->right->color;
            l->right->color = RED;
            return l;
        }

        Node* flip_colors(Node* node) {
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
            node->recalc_size();
            node->recalc_parents();
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
                delete node;
                return nullptr;
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
                    delete node;
                    return nullptr;
                }
                if (get_color(node->right) == BLACK && node->right != nullptr 
                        && get_color(node->right->left) == BLACK) {
                    node = move_red_right(node);
                }
                if (node->value < value) {
                    node->right = erase(node->right, value);
                } else {
                    Node* min_right_node = min_node(node->right);
                    node->value = min_right_node->value;
                    node->right = erase_min(node->right); 
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
