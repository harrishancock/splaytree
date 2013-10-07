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

    splaytree::set<int> st { v.begin(), v.end() };

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

    printf("Printing range [50,100]\n");
    for (auto cit = st.lower_bound(50); cit != st.upper_bound(100); ++cit) {
        printf("%d\n", *cit);
    }

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

    splaytree::set<int> st2 = {
        123, 456, 789
    };

    for (auto i : st2) {
        printf("%d\n", i);
    }

    assert(!st2.empty());
    st2.clear();
    assert(st2.empty());

    splaytree::map<std::string, int> sm;

    auto result = sm.insert(std::make_pair(std::string("zero"), 0));
    assert(result.second);

    sm.insert(std::make_pair(std::string("one"), 1));
    sm.insert(std::make_pair(std::string("two"), 2));
    sm.insert(std::make_pair(std::string("three"), 3));
    sm.insert(std::make_pair(std::string("four"), 4));
    sm.insert(std::make_pair(std::string("five"), 5));
    sm.insert(std::make_pair(std::string("six"), 6));

    sm["_hello"] = 654;
    sm["_world"] = 37;

    printf("sm.at(\"_hello\") == %d\n", sm.at("_hello"));

    try {
        sm.at("fart");
    }
    catch (std::out_of_range& exc) {
        printf("exception: %s\n", exc.what());
    }

    for (auto& pr : sm) {
        std::cout << pr.first << " : " << pr.second << '\n';
    }

    printf("erasing [\"four\",\"two\")\n");
    sm.erase(sm.lower_bound(std::string("four")), sm.lower_bound(std::string("two")));
    for (auto& pr : sm) {
        std::cout << pr.first << " : " << pr.second << '\n';
    }

    {
        splaytree::map<std::pair<int, std::string>, int> sm2;
        sm2[std::make_pair(3, std::string("aaa"))] = 3;
        sm2[std::make_pair(1, std::string("aaa"))] = 1;
        sm2[std::make_pair(2, std::string("aaa"))] = 2;
        sm2[std::make_pair(0, std::string("aaa"))] = 0;
        sm2[std::make_pair(4, std::string("aaa"))] = 4;

        for (auto& pair : sm2) {
            printf("(%d, %s) : %d\n", pair.first.first, pair.first.second.c_str(), pair.second);
        }
    }
}
