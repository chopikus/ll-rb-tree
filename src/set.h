#pragma once

#include <initializer_list>
#include <memory>

template <typename ValueType>
class Node {
    public:
    enum Color : bool { BLACK = false, RED = true };

    ValueType value;
    Color color;
    size_t size;

    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Node<ValueType>* parent{nullptr};

    Node(const ValueType& v): value{v} {}

    Node(const Node<ValueType>& another) : 
        value{another.value},
        color{another.color},
        size{another.size} {
        if (another.left) {
            left = std::make_unique<Node<ValueType>>(*another.left);
            left->parent = this;
        }
        if (another.right) {
            right = std::make_unique<Node<ValueType>>(*another.right);
            right->parent = this;
        }
    }

    Node<ValueType>& operator=(Node<ValueType> other) {
        swap(*this, other);
        return *this;
    }

    ~Node() = default;

    friend void swap(Node<ValueType>& first, Node<ValueType>& second) noexcept {
        using std::swap;
        swap(first.value, second.value);
        swap(first.color, second.color);
        swap(first.size, second.size);
        swap(first.left, second.left);
        swap(first.right, second.right);
        swap(first.parent, second.parent);
    }
};

template<class ValueType>
class Set {
    public:
        class iterator {
            public: 
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
                iterator(Node<ValueType>* node, const Set<ValueType>* set) : node(node), set(set) {};
                Node<ValueType>* node{nullptr};
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
            if (another.root) {
                root = new Node<ValueType>(*another.root);
            } else {
                root = nullptr;
            }
        }
    
        Set& operator=(const Set& another) {
            if (another.root) {
                root = new Node<ValueType>(*another.root);
            } else {
                root = nullptr;
            }
            return *this;
        }

        void insert(const ValueType& value) {
            if (!root) {
                root = new Node<ValueType>(value);
            } else {
                root = insert(root, value);
            }
            root->color = Color::BLACK;
        }

        void erase(const ValueType& value) {
            if (!root) {
                return;
            }
            if (get_color((root->left).get()) == Color::BLACK && 
                get_color((root->right).get()) == Color::BLACK) {
                root->color = Color::RED;
            }
            root = erase(root, value);
            if (root) {
                root->color = Color::BLACK;
            }
        }

        void erase(const iterator& iterator) {
            if (iterator.node) {
                erase(*iterator);
            }
        }

        ~Set() = default;

    private:
        using Color = typename Node<ValueType>::Color;

        Node<ValueType>* root{nullptr};

        Node<ValueType>* min_node(Node<ValueType>* node) const {
            if (!node)
                return nullptr;
            while ((node->left).get()) {
                node = (node->left).get();
            }
            return node;
        }

        Node<ValueType>* max_node(Node<ValueType>* node) const {
            if (!node)
                return nullptr;
            while ((node->right).get()) {
                node = (node->right).get();
            }
            return node;
        }
        
        Node<ValueType>* prev(Node<ValueType>* node) const {
            if (!node) {
                return max_node(root);
            }
            if (node -> left) {
                return max_node((node->left).get());
            }
            while (node -> parent) {
                if ((node -> parent -> right).get() == node) {
                    return node->parent;
                }
                node = node->parent;
            }
            return nullptr;
        }

        Node<ValueType>* next(Node<ValueType>* node) const {
            if ((node->right).get()) {
                return min_node((node->right).get());
            }
            while (node->parent) {
                if ((node->parent->left).get() == node) {
                    return node->parent;
                }
                node = node->parent;
            }
            return nullptr;
        }

        Color get_color(Node<ValueType>* node) const {
            if (!node)
                return Color::BLACK;
            return node->color;
        }
            
        Node<ValueType>* find(Node<ValueType>* node, const ValueType& value) const {
            if (!node) {
                return nullptr;
            }
            if (value < node->value) {
                return find((node->left).get(), value);
            } else if (node->value < value) {
                return find((node->right).get(), value);
            } else {
                return node;
            }
        }

        Node<ValueType>* lower_bound(Node<ValueType>* node, const ValueType& value) const {
            if (!node) {
                return nullptr;
            }
            Node<ValueType>* result = nullptr;
            while (node) {
                if (node->value < value) {
                    node = (node->right).get();
                } else {
                    result = node;
                    node = (node->left).get();
                }
            } 
            return result;
       } 

        Node<ValueType>* rotate_left(Node<ValueType>* node) {
            Node<ValueType>* r = (node->right).get();
            (node->right).get() = r->left;
            r->left = node;

            r->left->recalc_size();
            r->recalc_size();

            r->left->recalc_parents();
            r->recalc_parents();

            r->color = r->left->color;
            r->left->color = Color::RED;
            return r;
        }

        Node<ValueType>* rotate_right(Node<ValueType>* node) {
            Node<ValueType>* l = (node->left).get();
            (node->left).get() = l->right;
            l->right = node;

            l->right->recalc_size();
            l->recalc_size();

            l->right->recalc_parents();
            l->recalc_parents();

            l->color = l->right->color;
            l->right->color = Color::RED;
            return l;
        }

        Node<ValueType>* flip_colors(Node<ValueType>* node) {
            node->color = !node->color;
            node->left->color = !(node->left)->color;
            node->right->color = !(node->right)->color;
            return node;
        }

        Node<ValueType>* balance(Node<ValueType>* node) {
            if (get_color((node->left).get())==Color::BLACK && get_color((node->right).get())==Color::RED) {
                node = rotate_left(node);
            }
            if (get_color((node->left).get())==Color::RED && get_color((node->left).get()->left)==Color::RED) {
                node = rotate_right(node);
            }
            if (get_color((node->left).get())==Color::RED && get_color((node->right).get())==Color::RED) {
                node = flip_colors(node);
            } 
            node->recalc_size();
            node->recalc_parents();
            return node;
        }

        Node<ValueType>* insert(Node<ValueType>* node, const ValueType& value) {
            if (!node) {
                return new Node(value);
            }
            if (value < node->value) {
                node->left.reset(insert((node->left).get(), value));
            } else if (node->value < value) {
                node->right.reset(insert((node->right).get(), value));
            }
            return balance(node);
        }

        Node<ValueType>* move_red_left(Node<ValueType>* node) {
            flip_colors(node);
            if ((node->right).get() && get_color((node->right).get()->left) == Color::RED) {
                (node->right).get() = rotate_right((node->right).get());
                node = rotate_left(node);
                flip_colors(node);
            }
            return node;
        }

        Node<ValueType>* move_red_right(Node<ValueType>* node) {
            flip_colors(node);
            if ((node->left).get() && get_color((node->left).get()->left) == Color::RED) {
                node = rotate_right(node);
                flip_colors(node);
            }
            return node;
        }

        Node<ValueType>* erase_min(Node<ValueType>* node) {
            if (!(node->left).get()) {
                delete node;
                return nullptr;
            }

            if (get_color((node->left).get()) == Color::BLACK && (node->left).get() 
                    && get_color((node->left).get()->left) == Color::BLACK) {
                node = move_red_left(node);       
            }

            (node->left).get() = erase_min((node->left).get());
            return balance(node);
        }

        Node<ValueType>* erase(Node<ValueType>* node, const ValueType& value) {
            if (node == nullptr) {
                return nullptr;
            }
            if (value < node->value) {
                if (get_color((node->left).get()) == Color::BLACK && (node->left).get()
                          && get_color(((node->left).get()->left).get()) == Color::BLACK) {
                    node = move_red_left(node);
                }
                (node->left).get() = erase((node->left).get(), value);
            } else {
                if (get_color((node->left).get()) == Color::RED) {
                    node = rotate_right(node);
                }

                if (!(node->value < value) && !(value < node->value) 
                                                    && !(node->right).get()) {
                    delete node;
                    return nullptr;
                }

                if (get_color((node->right).get()) == Color::BLACK && (node->right).get() 
                          && get_color(((node->right).get()->left).get()) == Color::BLACK) {
                    node = move_red_right(node);
                }

                if (node->value < value) {
                    (node->right).get() = erase((node->right).get(), value);
                } else {
                    Node<ValueType>* min_right_node = min_node((node->right).get());
                    node->value = min_right_node->value;
                    node->right.reset(erase_min((node->right).get())); 
                }
            }
            return balance(node);
        } 
};