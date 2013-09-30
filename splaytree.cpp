#include "splaytree.hpp"

class node {
public:
    using value_type = Value;

    void splay () {
        while (m_parent) {
            if (m_parent->is_root()) {
                /* Zig step */
                rotate();
            }
            else if (this->is_left_child() == m_parent->is_left_child()) {
                /* Zig-zig step */
                m_parent->rotate();
                rotate();
            }
            else {
                /* Zig-zag step */
                rotate();
                rotate();
            }
        }
    }

    bool is_root () {
        return !m_parent;
    }

    bool is_left_child () {
        return m_parent && m_parent->m_left == this;
    }


    template <typename Predicate>
    static node* search (node* root, const Predicate& pred) {
        node* p = nullptr;
        while (root) {
            p = root;
            root = pred(p->m_value) ? p->m_left : p->m_right;
        }

        if (!p) {
            /* Empty tree */
            return nullptr;
        }

        p->splay();
        return p;
    }

    static node* join (node* lhs, node* rhs) {
        if (!lhs) {
            return rhs;
        }

        if (!rhs) {
            return lhs;
        }

        assert(lhs->is_root());
        assert(rhs->is_root());

        /* Find the largest element of the left-hand tree. */
        auto root = search(lhs, always_false());
        assert(!root->m_right);

        root->m_right = rhs;
        rhs->m_parent = root;
        return root;
    }

    template <typename Compare>
    static std::pair<node*,node*> split (node* root, const value_type& value, const Compare& comp) {
        root = search(root, compare_wrapper<Compare>(comp, value));

        node* lhs = nullptr;
        node* rhs = nullptr;

        if (root) {
            if (comp(value, root->m_value)) {
                lhs = root->m_left;
                rhs = root;
                lhs->m_parent = nullptr;
                rhs->m_left = nullptr;
            }
            else {
                lhs = root;
                rhs = root->m_right;
                lhs->m_right = nullptr;
                rhs->m_parent = nullptr;
            }
        }

        return std::make_pair(lhs, rhs);
    }

private:
    struct always_true {
        bool operator() (const Value&) {
            return true;
        }
    };

    struct always_false {
        bool operator() (const Value&) {
            return false;
        }
    };

    template <typename Compare>
    struct compare_wrapper {
        compare_wrapper (const Compare& comp, const Value& value) 
                : m_comp(comp), m_value(value) { }

        bool operator() (const Value& other) {
            return m_comp(m_value, other);
        }

        Compare m_comp;
        Value m_value;
    };

#define SPLAYTREE_rotate(rotdir, offdir) \
    { \
        assert(m_parent); \
        assert(m_parent->offdir == this); \
        \
        if (rotdir) { \
            rotdir->m_parent = m_parent; \
        } \
        m_parent->offdir = rotdir; \
        rotdir = m_parent; \
        \
        auto gparent = m_parent->m_parent; \
        if (gparent) { \
            (gparent->m_left == m_parent ? \
             gparent->m_left : gparent->m_right) = this; \
        } \
        m_parent = gparent; \
        rotdir->m_parent = this; \
    }

    void rotate_right () SPLAYTREE_rotate(m_right, m_left)
    void rotate_left () SPLAYTREE_rotate(m_left, m_right)

#undef SPLAYTREE_rotate

    void rotate () {
        if (this->is_left_child()) {
            rotate_right();
        }
        else {
            rotate_left();
        }
    }

    value_type value;
    
    node* m_parent;
    node* m_left;
    node* m_right;
};
