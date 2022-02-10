#include <iostream>
#include <cassert>
#include <stddef.h>
#include <vector>
#include <string>
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
                left = nullptr;
                delete right;
                right = nullptr;

                if (another.left) {
                    left = new Node();
                    left->copy(*(another.left));
                }

                if (another.right) {
                    right = new Node();
                    right->copy(*(another.right));
                }
                this->recalc_parents();
            }

            void clear() {
                if (left) {
                    left->clear();
                }
                if (right) {
                    right->clear();
                }
                delete this;
            }
        };
         
    public:
        class iterator {
            public: 
                iterator() = default;
                iterator(const iterator& another) {
                    node = another.node;
                    set = another.set;
                };
                const ValueType& operator*() const {
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
                    node = set->next(node);
                    return *this;
                }

                iterator operator++(int) {
                    iterator it = *this;
                    node = set->next(node); 
                    return it;
                }
                
                iterator operator--() {
                    node = set->prev(node);
                    return *this;
                }
                
                iterator operator--(int) { 
                    iterator it = *this;
                    node = set->prev(node);
                    return it;
                }

            private:
                iterator(Node* node, const Set* set) : node(node), set(set) {};
                Node* node = nullptr;
                const Set* set;

            friend class Set;
        }; 
        
        iterator begin() const {
            return iterator(min_node(root), this);
        }

        iterator end() const {
            return iterator(nullptr, this);
        }

        bool empty() const {
            return (!root);
        }

        size_t size() const {
            if (!root) {
                return 0;
            } else {
                return root->size;
            }
        }
        
        iterator find(const ValueType& value) const {
            return iterator(find(root, value), this);
        }
        
        iterator lower_bound(const ValueType& value) const {
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
            if (root == another.root) {
                return *this;
            }
            if (root) {
                root->clear();
                root = nullptr;
            }
            if (another.root) {
                root = new Node();
                root->copy(*(another.root));
            }
            return *this;
        }

        void insert(const ValueType& value) {
            if (!root) {
                root = new Node(value);
            } else {
                root = insert(root, value);
            }
            root->color = BLACK;
        }

        void erase(const ValueType& value) {
            if (!root) {
                return;
            }
            if (get_color(root->left) == BLACK && 
                get_color(root->right) == BLACK) {
                root->color = RED;
            }
            root = erase(root, value);
            if (root) {
                root->color = BLACK;
            }
        }

        void erase(const iterator& iterator) {
            if (iterator.node) {
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
            if (!node)
                return nullptr;
            while (node->left) {
                node = node->left;
            }
            return node;
        }

        Node* max_node(Node* node) const {
            if (!node)
                return nullptr;
            while (node->right) {
                node = node->right;
            }
            return node;
        }
        
        Node* prev(Node* node) const {
            if (!node) {
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
            if (!node)
                return BLACK;
            return node->color;
        }
            
        Node* find(Node* node, const ValueType& value) const {
            if (!node) {
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
            if (!node) {
                return nullptr;
            }
            Node* result = nullptr;
            while (node) {
                if (node->value < value) {
                    node = node->right;
                } else {
                    result = node;
                    node = node->left;
                }
            } 
            return result;
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
            if (!node) {
                return new Node(value);
            }
            if (value < node->value) {
                node->left = insert(node->left, value);
            } else if (node->value < value) {
                node->right = insert(node->right, value);
            }
            return balance(node);
        }

        Node* move_red_left(Node* node) {
            flip_colors(node);
            if (node->right && get_color(node->right->left) == RED) {
                node->right = rotate_right(node->right);
                node = rotate_left(node);
                flip_colors(node);
            }
            return node;
        }

        Node* move_red_right(Node* node) {
            flip_colors(node);
            if (node->left && get_color(node->left->left) == RED) {
                node = rotate_right(node);
                flip_colors(node);
            }
            return node;
        }

        Node* erase_min(Node* node) {
            if (!node->left) {
                delete node;
                return nullptr;
            }

            if (get_color(node->left) == BLACK && node->left 
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
                if (get_color(node->left) == BLACK && node->left
                          && get_color(node->left->left) == BLACK) {
                    node = move_red_left(node);
                }
                node->left = erase(node->left, value);
            } else {
                if (get_color(node->left) == RED) {
                    node = rotate_right(node);
                }

                if (!(node->value < value) && !(value < node->value) 
                                                    && !node->right) {
                    delete node;
                    return nullptr;
                }

                if (get_color(node->right) == BLACK && node->right 
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
