/* CS 4110
 * Author: Harris Hancock */

#ifndef SPLAYTREE_HPP
#define SPLAYTREE_HPP

#include <initializer_list>
#include <iterator>
#include <functional>

namespace detail {

template <typename Value>
struct splaytree_node;

template <typename Value>
struct splaytree_iterator
        : std::iterator<std::bidirectional_iterator_tag, Value> {
    // TODO
    reference operator* () const;

    /* TODO Prefix */
    splaytree_iterator<value_type>& operator++ ();

    /* Postfix */
    splaytree_iterator<value_type> operator++ (int) {
        auto ret = *this;
        ++*this;
        return ret;
    }
};

template <typename Value>
struct splaytree_const_iterator;

} // namespace detail

template <typename Value, typename Compare = std::less<value>>
class splaytree {
public:
    using key_type = Value;
    using key_compare = Compare;
    using value_type = key_type;
    using value_compare = key_compare;

    using reference = value_type&;
    using const_reference = const value_type&;

    using iterator = detail::splaytree_iterator<value_type>;
    using const_iterator = detail::splaytree_const_iterator<value_type>;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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

    // TODO
    splaytree (std::initializer_list<value_type> ilist,
               const value_compare& comp = value_compare());

    /* TODO initializer_list assignment operator */

    ~splaytree () {
        delete m_root;
    }

    // TODO
    iterator begin ();
    const_iterator begin () const;

    const_iterator cbegin () const {
        return begin();
    }

    // TODO
    iterator end ();
    const_iterator end () const;

    const_iterator cend () const {
        return end();
    }

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
    bool operator< (const splaytree<value_type>& other) const;
    bool operator> (const splaytree<value_type>& other) const;
    bool operator<= (const splaytree<value_type>& other) const;
    bool operator>= (const splaytree<value_type>& other) const;

    /* Unnecessary--I only provided this because the STL requires it. */
    void swap (splaytree& lhs, splaytree& rhs) {
        using std::swap;
        swap(lhs, rhs);
    }

    friend void swap (splaytree& lhs, splaytree& rhs) {
        using std::swap;
        swap(lhs.m_root, rhs.m_root);
    }

    splaytree& operator= (splaytree other) {
        using std::swap;
        swap(*this, other);
        return *this;
    }
    
    size_type size () const {
        return m_size;
    }

    size_type max_size () const {
        // Hell if I know.
        return static_cast<size_type>(-1);
    }

    bool empty () const {
        return !m_root;
    }

    reverse_iterator rbegin () {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin () const {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin () const {
        return rbegin();
    }

    reverse_iterator rend () {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend () const {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend () const {
        return rend();
    }

    key_compare key_comp () const {
        return m_comp;
    }

    value_compare value_comp () const {
        return m_comp;
    }

    // TODO
    template <typename... Args>
    std::pair<iterator, bool> emplace (Args&&... args);

    template <typename... Args>
    iterator emplace_hint (const_iterator, Args&&... args) {
        /* Ignore the hint for now. */
        return emplace(std::forward<Args>(args)...).first;
    }

    // TODO
    std::pair<iterator, bool> insert (const value_type& value);
    std::pair<iterator, bool> insert (value_type&& value);

    iterator insert (const_iterator, const value_type& value) {
        /* Ignore the hint for now. */
        return insert(value).first;
    }

    iterator insert (const_iterator, value_type&& value) {
        /* Ignore the hint for now. */
        return insert(value).first;
    }

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
    size_type erase (const value_type& value);

    // TODO
    iterator erase (const_iterator pos);

    // TODO
    iterator erase (const_iterator first, const_iterator last);
    
    void clear () {
        delete m_root;
        m_root = nullptr;
        m_size = 0;
    }

    // TODO
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

private:
    using node_type = detail::splaytree_node<value_type>;

    value_compare m_comp;
    size_type m_size;
    node_type* m_root;
};

#endif
