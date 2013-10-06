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

    splaytree::splaytree<int> st { v.begin(), v.end() };

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

    printf("Printing range [55,100]\n");
    for (auto cit = st.lower_bound(55); cit != st.upper_bound(100); ++cit) {
        printf("%d\n", *cit);
    }

    printf("erasing 53\n");
    assert(1 == st.erase(53));

    printf("Printing range [50,55)\n");
    for (auto cit = st.lower_bound(50); cit != st.lower_bound(55); ++cit) {
        printf("%d\n", *cit);
    }

    printf("Printing range (50,55]\n");
    for (auto cit = st.upper_bound(50); cit != st.upper_bound(55); ++cit) {
        printf("%d\n", *cit);
    }

    printf("\n");
    printf("st.count(33) == %zu\n", st.count(33));
    assert(st.end() == st.find(100));
    assert(st.end() != st.find(33));

    printf("st.size() = %zu\n", st.size());
    printf("erasing the first 30 elements ...\n");
    auto it = st.begin();
    for (int i = 0; i < 30; ++i) {
        it = st.erase(it);
    }
    printf("st.size() = %zu\n", st.size());
    for (auto i : st) {
        printf("%d\n", i);
    }

    ++it; ++it; ++it; ++it; ++it;
    auto last = it;
    ++last; ++last; ++last; ++last; ++last;
    printf("erasing another bunch\n");
    st.erase(it, last);
    printf("st.size() = %zu\n", st.size());
    for (auto i : st) {
        printf("%d\n", i);
    }

    printf("erasing nothing (begin)\n");
    st.erase(st.begin(), st.begin());
    printf("st.size() = %zu\n", st.size());

    printf("erasing nothing (end)\n");
    st.erase(st.end(), st.end());
    printf("st.size() = %zu\n", st.size());

    printf("erasing everything\n");
    st.erase(st.begin(), st.end());
    printf("st.size() = %zu\n", st.size());
    for (auto i : st) {
        printf("%d\n", i);
    }

    splaytree::splaytree<int> st2 = {
        123, 456, 789
    };

    for (auto i : st2) {
        printf("%d\n", i);
    }

    assert(!st2.empty());
    st2.clear();
    assert(st2.empty());

}
