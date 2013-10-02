#include "splaytree.hpp"

#include <cstdio>

#include <algorithm>
#include <vector>

int main () {
    std::vector<int> v;
    for (int i = 0; i < 58; ++i) {
        v.push_back(i);
    }
    std::random_shuffle(v.begin(), v.end());

    splaytree<int> st { v.begin(), v.end() };

    assert(!st.insert(30).second);

#if 0
    for (auto i : st) {
        printf("%d\n", i);
    }
#endif

    for (auto cit = st.cbegin(); cit != st.cend(); ++cit) {
        printf("%d\n", *cit);
    }

    printf("\n");
    assert(st.end() == st.find(100));
    assert(st.end() != st.find(33));

    splaytree<int> st2 = {
        123, 456, 789
    };

    for (auto i : st2) {
        printf("%d\n", i);
    }
}
