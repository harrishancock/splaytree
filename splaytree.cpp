#include "splaytree.hpp"

#include <cstdio>

int main () {
    splaytree<int> st;

    st.insert(3);
    st.insert(4);
    st.insert(32);
    st.insert(-4);
    st.insert(56);
    st.insert(11);

    for (auto i : st) {
        printf("%d\n", i);
    }
}
