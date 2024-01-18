#pragma once

#include <initializer_list>
#include <memory>
#include <iostream>

namespace llrb {

template<typename T>
concept Comparable =
    requires(T a, T b) {
        { a < b } -> std::convertible_to<bool>;
    };


template<Comparable ValueType>
class Node;
template<Comparable ValueType>
class Iterator;
template<Comparable ValueType>
class Set;

template <Comparable ValueType>
class Node {
    public:
    enum Color : bool { BLACK = false, RED = true };

    ValueType value;
    Color color;

    std::unique_ptr<Node> left{nullptr};
    std::unique_ptr<Node> right{nullptr};
    Node<ValueType>* parent{nullptr};

    Node(const ValueType& v): value{v}, color{Color::RED} {}

    Node(const Node<ValueType>& another) : 
        value{another.value},
        color{another.color} {
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

    Node(Node<ValueType>&& another) = default;
    Node<ValueType>& operator=(Node<ValueType>&& other) = default;

    ~Node() = default;

    friend void swap(Node<ValueType>& first, Node<ValueType>& second) noexcept {
        using std::swap;
        swap(first.value, second.value);
        swap(first.color, second.color);
        swap(first.left, second.left);
        swap(first.right, second.right);
        swap(first.parent, second.parent);
    }

    Node* l() const {
        return left.get();
    }

    Node* r() const {
        return right.get();
    }

    void recalc() {
        if (left) {
            left->parent = this;
        }

        if (right) {
            right->parent = this;
        }    
    }
};

template <Comparable ValueType>
class Iterator {
    public: 
        Iterator() = default;
        Iterator(const Iterator& another) {
            node = another.node;
            set = another.set;
        };
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

        Iterator operator++() {
            node = set->next(node);
            return *this;
        }

        Iterator operator++(int) {
            Iterator it = *this;
            node = set->next(node); 
            return it;
        }
        
        Iterator operator--() {
            node = set->prev(node);
            return *this;
        }
        
        Iterator operator--(int) { 
            Iterator it = *this;
            node = set->prev(node);
            return it;
        }

    private:
        Iterator(Node<ValueType>* node, const Set<ValueType>* set) : node(node), set(set) {};
        Node<ValueType>* node{nullptr};
        const Set<ValueType>* set{nullptr};

    friend class Set<ValueType>;
};

template<Comparable ValueType>
class Set {
    private:
        using Color = typename Node<ValueType>::Color;
        using NodePtr = std::unique_ptr<Node<ValueType>>;
        using RawNode = Node<ValueType>*;
    public:
        using iterator = Iterator<ValueType>;

        Set() {};

        Set(const std::initializer_list<ValueType> &list) {
            for (auto &element : list) {
                insert(element);
            }
        }

        Set(const Set<ValueType>& another) {
            if (another.root)
                root = std::make_unique<Node<ValueType>>(*another.root);
            _size = another._size;
        }

        Set& operator=(const Set<ValueType>& another) {
            if (another.root)
                root = std::make_unique<Node<ValueType>>(*another.root);
            _size = another._size;
            return *this;
        }

        Set(Set<ValueType>&& another) = default;
        Set& operator=(Set<ValueType>&& another) = default;

        ~Set() = default;
        
        void insert(const ValueType& value) {
            ++_size;
            root = insert(std::move(root), value);
            root->color = Color::BLACK;
            root->parent = nullptr;
        }

        void erase(const ValueType& value) {
            if (!root) {
                return;
            }
            if (!is_red(root->l()) && !is_red(root->r())) {
                root->color = Color::RED;
            }
            root = erase(std::move(root), value);
            if (root) {
                root->color = Color::BLACK;
                root->parent = nullptr;
            }
        }
        
        iterator begin() const {
            return iterator(min_node(root.get()), this);
        }

        iterator end() const {
            return iterator(nullptr, this);
        }

        bool empty() const {
            return (!root);
        }

        size_t size() const {
            return _size; 
        }

        iterator find(const ValueType& value) const {
            return iterator(find(root.get(), value), this);
        }

        iterator lower_bound(const ValueType& value) const {
            return iterator(lower_bound(root.get(), value), this);
        }

    private:
        NodePtr root{nullptr};
        size_t _size{0};

        inline bool is_red(RawNode node) const {
            if (!node)
                return false;
            return node->color == Color::RED;
        }

        RawNode min_node(RawNode node) const {
            if (!node)
                return nullptr;
            while (node->l()) {
                node = node->l();
            }
            return node;
        }

        RawNode max_node(RawNode node) const {
            if (!node)
                return nullptr;
            while (node->r()) {
                node = node->r();
            }
            return node;
        }

        RawNode prev(RawNode node) const {
            if (!node) {
                return max_node(root.get());
            }
            if (node -> left) {
                return max_node(node->l());
            }
            while (node -> parent) {
                if (node -> parent -> r() == node) {
                    return node->parent;
                }
                node = node->parent;
            }
            return nullptr;
        }
        RawNode next(RawNode node) const {
            if (node->r()) {
                return min_node(node->r());
            }
            while (node->parent) {
                if (node->parent->l() == node) {
                    return node->parent;
                }
                node = node->parent;
            }
            return nullptr;
        }
        
        RawNode find(RawNode node, const ValueType& value) const {
            while (node) {
                if (value < node->value) {
                    node = node->l();
                } else if (node->value < value) {
                    node = node->r();
                } else {
                    return node;
                }
            }
            return nullptr;
        }

        RawNode lower_bound(RawNode node, const ValueType& value) const {
            if (!node) {
                return nullptr;
            }
            RawNode result{nullptr};
            while (node) {
                if (node->value < value) {
                    node = node->r();
                } else {
                    result = node;
                    node = node->l();
                }
            } 
            return result;
        }

        [[nodiscard]]
        NodePtr rotate_left(NodePtr a) {
            /* Assuming a and b exist. */
            NodePtr b{std::move(a->right)};
            NodePtr l_a{std::move(a->left)};
            NodePtr g_a_l_b{std::move(b->left)};
            NodePtr g_b{std::move(b->right)};
            
            a->left = std::move(l_a);
            a->right = std::move(g_a_l_b);
            b->right = std::move(g_b);
            b->left = std::move(a); /* a is invalid from here */
            
            b->color = b->left->color;
            b->left->color = Color::RED;
            b->left->recalc();
            b->recalc();

            return b;
        };

        [[nodiscard]]
        NodePtr rotate_right(NodePtr b) {
            /* Assuming a and b exist. */
            NodePtr a{std::move(b->left)};
            NodePtr g_b{std::move(b->right)};
            NodePtr l_a{std::move(a->left)};
            NodePtr g_a_l_b{std::move(a->right)};

            b->left = std::move(g_a_l_b);
            b->right = std::move(g_b);
            a->left = std::move(l_a);
            a->right = std::move(b); /* b invalid from here*/

            a->color = a->right->color;
            a->right->color = Color::RED;
            a->right->recalc();
            a->recalc();
            
            return a;
        }

        [[nodiscard]]
        NodePtr flip_colors(NodePtr node) {
            /* Assuming that children exist. */
            auto flip_color = [](Color color) -> Color {
                if (color == Color::RED)
                    return Color::BLACK;
                return Color::RED;
            };
            node->color = flip_color(node->color);
            node->l()->color = flip_color(node->l()->color);
            node->r()->color = flip_color(node->r()->color);
            return node;
        }

        [[nodiscard]]
        NodePtr fix_up(NodePtr node) {
            if (!node)
                return nullptr;
            
            if (is_red(node->r()) && !is_red(node->l())) {
                node = rotate_left(std::move(node));
            }
            if (is_red(node->l()) && is_red(node->l()->l())) {
                node = rotate_right(std::move(node));
            }
            if (is_red(node->l()) && is_red(node->r())) {
                node = flip_colors(std::move(node));
            }
            return node;
        }

        [[nodiscard]]
        NodePtr insert(NodePtr node, const ValueType& value) {
            if (!node) {
                return std::make_unique<Node<ValueType>>(value);
            }

            if (value < node->value) {
                node->left = insert(std::move(node->left), value);
            } else if (node->value < value) {
                node->right = insert(std::move(node->right), value);
            }
            node->recalc();
            return fix_up(std::move(node));
        }

        [[nodiscard]]
        NodePtr move_red_left(NodePtr node) {
            node = flip_colors(std::move(node));
            if (node->r() && is_red(node->r()->l())) {
                node->right = rotate_right(std::move(node->right));
                node = rotate_left(std::move(node));
                node = flip_colors(std::move(node));
            }
            return node;
        }

        [[nodiscard]]
        NodePtr move_red_right(NodePtr node) {
            node = flip_colors(std::move(node));
            if (node->l() && is_red(node->l()->l())) {
                node = rotate_right(std::move(node));
                node = flip_colors(std::move(node));
            }
            return node;
        }

        [[nodiscard]]
        NodePtr erase_min(NodePtr node) {
            if (!node || !node->left) {
                return nullptr;
            }
            if (node->l() && !is_red(node->l()) && !is_red(node->l()->l())) {
                node = move_red_left(std::move(node));
            }
            node->left = erase_min(std::move(node->left));
            node->recalc();
            return fix_up(std::move(node));
        }

        [[nodiscard]]
        NodePtr erase(NodePtr node, const ValueType& value) {
            if (!node) {
                return nullptr;
            }
            if (value < node->value)
            {
                if (node->l() && !is_red(node->l()) && !is_red(node->l()->l())) {
                    node = move_red_left(std::move(node));
                }
                node->left = erase(std::move(node->left), value);
            }
            else
            {
                if (is_red(node->l())) {
                    node = rotate_right(std::move(node));
                }
                
                if (!(node->value < value) && !node->r()) {
                    --_size;
                    return nullptr;
                }
                
                if (node->r() && !is_red(node->r()) && !is_red(node->r()->l())) {
                    node = move_red_right(std::move(node));
                }

                if (node->value < value) {
                    node->right = erase(std::move(node->right), value);
                } else {
                    RawNode min_right_node = min_node(node->r());
                    node->value = min_right_node->value;
                    node->right = erase_min(std::move(node->right));
                }
            }
            node->recalc();
            return fix_up(std::move(node));
        }
    friend class Node<ValueType>;
    friend class Iterator<ValueType>;
};

}
