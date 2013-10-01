/* CS 4110
 * Author: Harris Hancock */

#ifndef SPLAYTREE_HPP
#define SPLAYTREE_HPP

#include <iostream>

#include <initializer_list>
#include <iterator>
#include <functional>

namespace detail {

template <typename Value>
class node {
public:
    node (const Value& value, node* left, node* right)
            : m_value(value)
            , m_left(left)
            , m_right(right) { }

    node (Value&& value, node* left, node* right)
            : m_value(std::forward<Value>(value))
            , m_left(left)
            , m_right(right) { }

    ~node () {
        delete m_left;
        delete m_right;
    }

    Value& value () {
        return m_value;
    }

    bool is_root () {
        return !m_parent;
    }

    bool is_left_child () {
        return m_parent && m_parent->m_left == this;
    }

    template <typename Predicate>
    static node* search_no_splay (node* root, const Predicate& pred) {
        node* p = nullptr;

        while (root) {
            p = root;
            root = pred(p->m_value) ? p->m_left : p->m_right;
        }

        return p;
    }

    template <typename Predicate>
    static node* search (node* root, const Predicate& pred) {
        node* p = search_no_splay(root, pred);

        if (p) {
            p->splay();
        }

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
        auto root = search(lhs, [] (const Value&) { return false; });
        assert(!root->m_right);

        root->m_right = rhs;
        rhs->m_parent = root;
        return root;
    }

    /* Convert to non-static function */
    template <typename Compare>
    static std::pair<node*,node*> split (node* root, const Value& value, const Compare& comp) {
        root = search(root,
                [&comp, &value] (const Value& other) {
                    return comp(value, other);
                });

        node* lhs = nullptr;
        node* rhs = nullptr;

        if (root) {
            /* Break one of the child links so that the left-hand side
             * has values less than or equal to the search key, and the
             * right-hand side has values greater than the search key. */
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

    static node* increment (node* s) {
        if (!s) {
            return nullptr;
        }

        if (s->m_right) {
            /* Find the smallest element in the right child. */
            return search_no_splay(s->m_right, [] (const Value&) { return true; });
        }

        if (s->is_left_child()) {
            return s->m_parent;
        }

        while (s->m_parent && !s->is_left_child()) {
            s = s->m_parent;
        }

        return s->m_parent;
    }

private:
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
        assert(!this->is_root());

        if (this->is_left_child()) {
            rotate_right();
        }
        else {
            rotate_left();
        }
    }

    void splay () {
        while (!this->is_root()) {
            if (!m_parent->is_root()) {
                if (this->is_left_child() == m_parent->is_left_child()) {
                    m_parent->rotate();
                }
                else {
                    rotate();
                }
            }
            rotate();
        }
    }

    Value m_value;
    
    node* m_parent;
    node* m_left;
    node* m_right;
};

template <typename Value>
class iterator
        : public std::iterator<std::forward_iterator_tag, Value> {
public:
    using node_type = node<Value>;

    iterator (node_type* node = nullptr)
            : m_node(node) { }

    bool operator!= (const iterator& other) const {
        return m_node != other.m_node;
    }

    reference operator* () {
        return m_node->value();
    }

    pointer operator-> () {
        return &m_node->value();
    }

    iterator& operator++ () {
        m_node = node_type::increment(m_node);
        return *this;
    }

    /* Postfix */
    iterator operator++ (int) {
        auto ret = *this;
        ++*this;
        return ret;
    }

private:
    node_type* m_node;
};

#if 0
template <typename Value>
struct const_iterator;
#endif

} // namespace detail

template <typename Value, typename Compare = std::less<Value>>
class splaytree {
public:
    using key_type = Value;
    using key_compare = Compare;
    using value_type = key_type;
    using value_compare = key_compare;

    using reference = value_type&;
    using const_reference = const value_type&;

    using iterator = detail::iterator<value_type>;
#if 0
    using const_iterator = detail::const_iterator<value_type>;
#endif

    using difference_type = ptrdiff_t;
    using size_type = size_t;

    /* Default constructor */
    explicit splaytree (const value_compare& comp = value_compare())
            : m_comp(comp)
            , m_size(0)
            , m_root(nullptr) { }

    /* Copy constructor */
    /* TODO think about exception safety here */
    splaytree (const splaytree& other)
            : splaytree(other.begin(), other.end(), other.value_comp()) { }

    /* Move constructor */
    splaytree (splaytree&& other) : splaytree() {
        using std::swap;
        swap(*this, other);
    }

    template <typename Iter>
    splaytree (Iter first, Iter last, const value_compare& comp = value_compare())
            : splaytree(comp) {
        /* TODO think about exception safety here */
        insert(first, last);
    }

    splaytree (std::initializer_list<value_type> ilist,
               const value_compare& comp = value_compare())
            : splaytree(ilist.begin(), iliest.end(), comp) { }

    ~splaytree () {
        delete m_root;
    }

    iterator begin () {
        auto p = node_type::search_no_splay(m_root, [] (const value_type&) {
                return true;
            });
        return iterator(p);
    }

    // TODO
#if 0
    const_iterator begin () const;

    const_iterator cbegin () const {
        return begin();
    }
#endif

    iterator end () {
        return iterator(nullptr);
    }

    // TODO
#if 0
    const_iterator end () const;

    const_iterator cend () const {
        return end();
    }
#endif

    bool operator== (const splaytree<value_type>& other) const {
        if (size() != other.size()) {
            return false;
        }

        return std::equal(begin(), end(), other.begin());
    }

    bool operator!= (const splaytree<value_type>& other) const {
        return !(*this == other);
    }

    // TODO
#if 0
    bool operator< (const splaytree<value_type>& other) const;
    bool operator> (const splaytree<value_type>& other) const;
    bool operator<= (const splaytree<value_type>& other) const;
    bool operator>= (const splaytree<value_type>& other) const;
#endif

    /* Unnecessary--I only provided this because the STL requires it. */
    void swap (splaytree& other) {
        using std::swap;
        swap(*this, other);
    }

    friend void swap (splaytree& lhs, splaytree& rhs) {
        using std::swap;
        swap(lhs.m_comp, rhs.m_comp);
        swap(lhs.m_size, rhs.m_size);
        swap(lhs.m_root, rhs.m_root);
    }

    splaytree& operator= (splaytree other) {
        using std::swap;
        swap(*this, other);
        return *this;
    }
    
    splaytree& operator= (std::initializer_list<value_type> ilist) {
        return *this = splaytree(ilist, m_comp);
    }

    size_type size () const {
        return m_size;
    }

    size_type max_size () const {
        // Hell if I know.
        return std::numeric_limits<size_type>::max();
    }

    bool empty () const {
        return !m_root;
    }

    key_compare key_comp () const {
        return m_comp;
    }

    value_compare value_comp () const {
        return m_comp;
    }

    // TODO
#if 0
    template <typename... Args>
    std::pair<iterator, bool> emplace (Args&&... args);

    template <typename... Args>
    iterator emplace_hint (const_iterator, Args&&... args) {
        /* Ignore the hint for now. */
        return emplace(std::forward<Args>(args)...).first;
    }
#endif

    std::pair<iterator, bool> insert (const value_type& value) {
        /* printf */
        std::cout << "inserting " << value;

        node_type* lhs;
        node_type* rhs;
        std::tie(lhs, rhs) = node_type::split(m_root, value, m_comp);

        bool success = false;
        /* We already know that the left-hand side has values less than or
         * equal to our search key, by the definition of split(). One more
         * comparison will tell us if the root of the left-hand side is equal
         * to our search key. */
        if (!lhs || m_comp(lhs->value(), value)) {
            m_root = new node_type (value, lhs, rhs);
            success = true;
            m_size++;
        }
        else {
            /* Key already exists--put our tree back together. */
            m_root = node_type::join(lhs, rhs);
        }

        return std::make_pair(iterator(m_root), success);
    }

#if 0
    std::pair<iterator, bool> insert (value_type&& value);

    iterator insert (const_iterator, const value_type& value) {
        /* Ignore the hint for now. */
        return insert(value).first;
    }

    iterator insert (const_iterator, value_type&& value) {
        /* Ignore the hint for now. */
        return insert(value).first;
    }
#endif

    template <typename Iter>
    void insert (Iter first, Iter last) {
        while (first != last) {
            insert(*first++);
        }
    }

    void insert (std::initializer_list<value_type> ilist) {
        insert(ilist.begin(), ilist.end());
    }

    // TODO
#if 0
    size_type erase (const value_type& value);

    iterator erase (const_iterator pos);

    iterator erase (const_iterator first, const_iterator last);
#endif
    
    void clear () {
        delete m_root;
        m_root = nullptr;
        m_size = 0;
    }

    // TODO
#if 0
    iterator find (const value_type& value);
    const_iterator find (const value_type& value) const;

    size_type count (const value_type& value);

    iterator lower_bound (const value_type& value);
    const_iterator lower_bound (const value_type& value) const;

    iterator upper_bound (const value_type& value);
    const_iterator upper_bound (const value_type& value) const;

    std::pair<iterator, iterator> equal_range (const value_type& value);
    std::pair<const_iterator, const_iterator>
    equal_range (const value_type& value) const;
#endif

private:
    using node_type = detail::node<value_type>;

    value_compare m_comp;
    size_type m_size;
    node_type* m_root;
};

#endif
