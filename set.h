#include <iostream>
#include <cassert>
#include <stddef.h>
#include <initializer_list>

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
            size_t size = 1;
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
        class iterator {
            public: 
                iterator() = default;
                iterator(const iterator& another) = default;
                const ValueType& operator*() const {
                    assert(*this != set_->end());
                    return node->value;             
                }
                
                const ValueType* operator->() const {
                    return &(node->value);
                }                

                bool operator!=(const iterator& another) const {
                    return node != another.node;
                }
 
                bool operator==(const iterator& another) const {
                    return node == another.node;
                }

                iterator operator++() {
                    iterator it = *this;
                    node = set_->next(node); 
                    return it;
                }

                iterator operator++(int) {
                    node = set_->next(node);
                    return *this;
                }
                
                iterator operator--() {
                    iterator it = *this;
                    node = set_->prev(node);
                    return it;
                }
                
                iterator operator--(int) {
                    node = set_->prev(node);
                    return *this; 
                }

            private:
                iterator(Node* node, const Set* set) : node(node), set_(set) {};
                Node* node = nullptr;
                const Set* set_;

            friend class Set;
        }; 
        
        iterator begin() const {
            std::cerr << "called begin!\n";
            return iterator(min_node(root), this);
        }

        iterator end() const {
            std::cerr << "called end!\n";
            return iterator(nullptr, this);
        }

        bool empty() const {
            std::cerr << "called empty!\n";
            return (root == nullptr);
        }

        size_t size() const {
            std::cerr << "called size!\n";
            if (root == nullptr) {
                return 0;
            } else {
                return root->size;
            }
        }
        
        iterator find(const ValueType& value) const {
            std::cerr << "called find!\n";
            return iterator(find(root, value), this);
        }
        
        iterator lower_bound(const ValueType& value) const {
            std::cerr << "called lower bound!\n";
            return iterator(lower_bound(root, value), this);
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
            std::cerr << "inserting!" << std::endl;
            if (root == nullptr) {
                root = new Node(value);
            } else {
                root = insert(root, value);
            }
            root->color = BLACK;
        }

        void erase(const ValueType& value) {
            std::cerr << "erasing!" << std::endl;
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

        void erase(const iterator& iterator) {
            std::cerr << "erasing_it!" << std::endl;
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
            if (value < node->value) {
                if (node->left == nullptr) {
                    node->left = new Node(value);
                } else {
                    node->left = insert(node->left, value);
                }
            } else if (node->value < value) {
                if (node->right == nullptr) {
                    node->right = new Node(value);
                } else {
                    node->right = insert(node->right, value);
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
