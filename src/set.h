#pragma once

#include <initializer_list>
#include <memory>
#include <iostream>

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

    Node(const ValueType& v): value{v}, color{Color::RED} {}

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

    Node* l() const {
        return left.get();
    }

    Node* r() const {
        return right.get();
    }

    void recalc() {
        /* Recalculates parent property for children, as well as size for the node. */
        size = 1;
        
        if (left) {
            left->parent = this;
            size += left->size;
        }

        if (right) {
            right->parent = this;
            size += right->size;
        }    
    }
};



template<class ValueType>
class Set {
    public:
        Set() {}
        ~Set() = default;

        using Color = typename Node<ValueType>::Color;
        using NodePtr = std::unique_ptr<Node<ValueType>>;
        using RawNode = Node<ValueType>*;
        
        void insert(const ValueType& value) {
            root = insert(std::move(root), value);
            root->color = Color::BLACK;
        }
        
        RawNode find_node(const ValueType& value) {
            return find(root.get(), value);
        }

        NodePtr root{nullptr};
        inline bool is_red(RawNode node) const {
            if (!node)
                return false;
            return node->color == Color::RED;
        }

        NodePtr min_node(NodePtr node) const;
        NodePtr max_node(NodePtr node) const;
        NodePtr prev(NodePtr node) const;
        NodePtr next(NodePtr node) const;

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

        NodePtr lower_bound(NodePtr node, const ValueType& value) const;

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
            
            b->left->color = Color::RED;
            b->left->recalc();
            b->recalc();

            return b;
        };

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

            a->right->color = Color::RED;

            a->right->recalc();
            a->recalc();
            
            return a;
        }

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

        NodePtr insert(NodePtr node, const ValueType& value) {
            if (!node) {
                return std::make_unique<Node<ValueType>>(value);
            }

            if (is_red(node->l()) && is_red(node->r())) {
                node = flip_colors(std::move(node));
            }

            if (value < node->value) {
                node->left = insert(std::move(node->left), value);
            } else if (node->value < value) {
                node->right = insert(std::move(node->right), value);
            }

            if (is_red(node->r()) && !is_red(node->l())) {
                node = rotate_left(std::move(node));
            }
            if (is_red(node->l()) && is_red(node->l()->l())) {
                node = rotate_right(std::move(node));
            }

            return node;
        }

        NodePtr move_red_left(NodePtr node);
        NodePtr move_red_right(NodePtr node);
        NodePtr erase_min(NodePtr node);
        NodePtr erase(NodePtr node, const ValueType& value);
};