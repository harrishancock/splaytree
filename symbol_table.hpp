#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include "splaytree.hpp"

#include <deque>

using scope_id = unsigned;
using identifier = std::string;
using id_key = std::pair<scope_id, identifier>;

struct id_record {
    /* placeholder until we have stuff to put here */
    int reference_count = 0;
};

using symbol_table = splaytree::map<id_key, id_record>;

/* Unify symbol table and scope management into a single class. */
class symbol_table_scope_manager {
public:
    using iterator = symbol_table::iterator;
    using const_iterator = symbol_table::const_iterator;

    void open_scope () {
        m_active_scopes.push_front(m_next_scope_id++);
    }

    void close_scope () {
        m_active_scopes.pop_front();
    }

    /* Get an iterator to the first symbol in any scope.
     *
     * Providing these breaks encapsulation in a way, though typedefing our
     * iterators to symbol_table's iterators already broke it. Nevertheless,
     * we need an end() iterator so find can return null values, and if we
     * supply an end(), we might as well supply a begin(). */
    iterator begin () {
        return m_symbol_table.begin();
    }

    /* Get an iterator to the first symbol in scope sid. */
    const_iterator begin (scope_id sid) const {
        auto key = std::make_pair(sid, std::string());
        return m_symbol_table.lower_bound(key);
    }

    /* Get an iterator to one past the last symbol in any scope. */
    iterator end () {
        return m_symbol_table.end();
    }

    /* Get an iterator to one past the last symbol in scope sid. */
    const_iterator end (scope_id sid) const {
        auto key = std::make_pair(sid + 1, std::string());
        return m_symbol_table.lower_bound(key);
    }

    std::pair<iterator, bool> insert (const identifier& id) {
        auto key = std::make_pair(m_active_scopes.front(), id);
        auto value = std::make_pair(key, id_record());
        return m_symbol_table.insert(value);
    }

    /* Search through each active scope until we find a match for this
     * identifier. If no match, return a one-past-the-end iterator. */
    iterator find (const identifier& id) {
        for (auto sid : m_active_scopes) {
            auto key = std::make_pair(sid, id);
            auto it = m_symbol_table.find(key);
            if (m_symbol_table.end() != it) {
                return it;
            }
        }

        /* Exhausted our active scopes, return end() to signify not found. */
        return end();
    }

    void display (FILE* file = stdout) const {
        static const int cols = 78;
        static const std::string title { "SYMBOL TABLE" };

        /* Print the title, surrounded by equals signs. */
        const auto num_equals = cols - title.length() - 2;
        for (int i = 0; i < num_equals / 2; ++i) {
            fputc('=', file);
        }
        fprintf(file, " %s ", title.c_str());
        for (int i = 0; i < num_equals / 2; ++i) {
            fputc('=', file);
        }
        /* The two for loops above use integer division, which introduces a
         * parity issue if the number of equals signs we wanted to print was
         * odd. Account for this. */
        if (1 & num_equals) {
            fputc('=', file);
        }
        fputc('\n', file);

        /* And finally print the scopes, in order. */
        for (scope_id i = 0; i < m_next_scope_id; ++i) {
            printf("Scope %d:\n", i);
            for (auto it = begin(i); end(i) != it; ++it) {
                auto& lexeme = it->first.second;
                auto& reference_count = it->second.reference_count;
                fprintf(file, "\t%s : reference_count<%d>\n", lexeme.c_str(), reference_count);
            }
        }
    }

private:
    scope_id m_next_scope_id = 0;

    /* std::stack would be a more logical choice for the active scope stack,
     * but it does not support iteration, which we need. std::deque does.
     * Another potential choice might be std::forward_list, whose interface is
     * a bit leaner, but for scope_ids, the memory allocation overhead of a
     * linked list is overkill. */
    std::deque<scope_id> m_active_scopes;
    symbol_table m_symbol_table;
};

#endif
