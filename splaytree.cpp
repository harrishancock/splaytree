#include "splaytree.hpp"

#include <cstdio>

#include <algorithm>
#include <vector>

int main () {
    std::vector<int> v;
    for (int i = -29; i < 29; ++i) {
        v.push_back(i);
    }
    std::random_shuffle(v.begin(), v.end());

    splaytree<int> st { v.begin(), v.end() };

    assert(!st.insert(0).second);

    for (auto i : st) {
        printf("%d\n", i);
    }
}
