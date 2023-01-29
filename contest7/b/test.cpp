#include <catch.hpp>
#include <set.h>
#include <set>
#include <util.h>

template<class T>
void Correctness(Set<T>& set, std::vector<T>& data) {
    for (auto num : data) {
        set.insert(num);
        REQUIRE(set.size() == 15);
        REQUIRE(*set.find(num) == num);
        REQUIRE(*set.lower_bound(num) == num);
        if (num == 1) {
            REQUIRE(*set.lower_bound(num - 1) == num);
        } else {
            REQUIRE(*set.lower_bound(num - 1) == num - 1);
        }
    }

    for (auto num : data) {
        REQUIRE(set.find(num + 15) == set.end());
        REQUIRE(*set.find(num) == num);
        REQUIRE(set.find(num - 15) == set.end());
        set.erase(num + 16);
        REQUIRE(set.size() == 15);
    }

    int size = 14;
    for (auto num : data) {
        set.erase(num);
        REQUIRE(set.find(num) == set.end());
        REQUIRE(set.size() == size--);
    }
    REQUIRE(set.empty());
}

TEST_CASE("set") {
    SECTION("Correctness init list") {
        std::vector<int> data(15);
        std::iota(data.begin(), data.end(), 1);
        Set<int> set{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        Correctness(set, data);
    }

    SECTION("Correctness array list") {
        std::vector<int> data(15);
        std::iota(data.begin(), data.end(), 1);
        Set<int> set(data.begin(), data.end());
        Correctness(set, data);
    }

    SECTION("iterator list") {
        std::vector<int> data(7);
        std::iota(data.begin(), data.end(), 1);
        do {
            Set<int> set(data.begin(), data.end());
            auto beg = set.begin();
            auto end = set.end();

            auto sorted = data;
            std::sort(sorted.begin(), sorted.end());

            auto sorted_it = sorted.begin();
            while (beg != end && sorted_it != sorted.end()) {
                REQUIRE((*beg) == *sorted_it);
                ++beg;
                ++sorted_it;
            }
            if (sorted_it != sorted.end() || beg != end) {
                REQUIRE(false);
            }
        } while (std::next_permutation(data.begin(), data.end()));
    }
}

TEST_CASE("set copy") {
    int elems[] = {3, 3, -3, 6, 0, 0, 17, -5, 4, 2};
    Set<int> s1(elems, elems + 10);
    Set<int> s2 = s1;
    s2.insert(5);
    s2.insert(18);
    s2.insert(-2);

    REQUIRE(s1.find(5) == s1.end());
    REQUIRE(s1.find(18) == s1.end());
    REQUIRE(s1.find(-2) == s1.end());
    for (auto num : elems) {
        REQUIRE(*s1.find(num) == num);
        REQUIRE(*s2.find(num) == num);
    }

    s1.erase(0);
    REQUIRE(s1.find(0) == s1.end());
    REQUIRE(*s2.find(0) == 0);
}

TEST_CASE("set copy assign") {
    int elems[] = {3, 3, -3, 6, 0, 0, 17, -5, 4, 2};
    Set<int> s1(elems, elems + 10);
    Set<int> s2;
    s2.insert(100);
    s2 = s1;
    REQUIRE(s2.find(100) == s1.end());

    s2.insert(5);
    s2.insert(18);
    s2.insert(-2);

    REQUIRE(s1.find(5) == s1.end());
    REQUIRE(s1.find(18) == s1.end());
    REQUIRE(s1.find(-2) == s1.end());
    for (auto num : elems) {
        REQUIRE(*s1.find(num) == num);
        REQUIRE(*s2.find(num) == num);
    }

    s1.erase(0);
    REQUIRE(s1.find(0) == s1.end());
    REQUIRE(*s2.find(0) == 0);
}

TEST_CASE("set stress") {
    std::vector<int> data(100000);
    std::iota(data.begin(), data.end(), 1);
    RandomGenerator gen;
    gen.Shuffle(data.begin(), data.end());
    /*
        Create: 3182
        Copy: 1410
     */
    Log l;
    Set<int> set(data.begin(), data.end());
    auto dur = l.GetDuration();
    std::cout << "Create: " << dur << std::endl;

    {
        Log logger;
        Set<int> copy = set;
        auto dur = logger.GetDuration();
        std::cout << "Copy: " << dur << std::endl;
    }

    {
        auto copy = data;
        std::sort(copy.begin(), copy.end());
        Log log;

        auto beg = set.begin();
        int i = 0;
        while (beg != set.end()) {
            REQUIRE(copy[i++] == *beg);
            ++beg;
        }
        auto dur = log.GetDuration();
        std::cout << "Iter: " << dur << std::endl;
    }
}

TEST_CASE("set stress op") {
    std::vector<int> data(100000);
    std::iota(data.begin(), data.end(), 1);
    RandomGenerator gen;
    gen.Shuffle(data.begin(), data.end());

    {
        Set<int> set(data.begin(), data.end());
        Log logger;
        for (auto num : data) {
            REQUIRE(*set.find(num) == num);
        }
        auto dur = logger.GetDuration();
        /* Find: 3222
           Find: 353 (стало)
        */
        std::cout << "Find: " << dur << std::endl;
    }
    {
        Set<int> set;
        std::vector<int> values;
        for (int i = 0; i < 100'000; ++i) {
            set.insert(i * 5);
            values.push_back(i * 5);
        }

        Log logger;
        for (auto num : values) {
            REQUIRE(*set.lower_bound(num - 1) == num);
        }
        auto dur = logger.GetDuration();
        /* Lower_b: 2789
           Lower_b: 369 (стало)
         */
        std::cout << "Lower_b: " << dur << std::endl;
    }
    {
        Set<int> set;
        std::vector<int> values;
        for (int i = 0; i < 200'000; ++i) {
            set.insert(i * 5);
            values.push_back(i * 5);
        }

        Log logger;
        for (auto num : values) {
            set.erase(num - 1);
            set.erase(num);
            set.erase(num + 1);
            REQUIRE(set.find(num) == set.end());
        }
        auto dur = logger.GetDuration();
        /* Remove: 2672
           Remove: 667 (стало)
         */
        std::cout << "Remove: " << dur << std::endl;
    }
}


/*
COMAPRE std::set
Insert: 200 3263
ctor non sort: 205 3114
ctor sort: 66 1290
Iter: 57 120
Remove: 178 963
 */
TEST_CASE("Compare set") {
    std::cout << '\n' << "COMAPRE std::set" << std::endl;
    std::vector<int> data(100'000);
    std::iota(data.begin(), data.end(), 5);
    RandomGenerator gen;
    gen.Shuffle(data.begin(), data.end());

    {
        Log logger;
        std::set<int> std_set;
        for (auto num : data) {
            std_set.insert(num);
        }
        auto dur_set = logger.GetDuration();

        Log log;
        Set<int> set;
        for (auto num : data) {
            set.insert(num);
        }
        auto dur = log.GetDuration();
        std::cout << "Insert: " << dur_set << " " << dur << std::endl;
    }
    {
        /* ctor not sorted */
        Log logger;
        std::set<int> std_set(data.begin(), data.end());
        auto dur_set = logger.GetDuration();

        Log log;
        Set<int> set(data.begin(), data.end());
        auto dur = log.GetDuration();
        std::cout << "ctor non sort: " << dur_set << " " << dur << std::endl;
    }

    {
        /* ctor sorted */
        auto copy = data;
        std::sort(copy.begin(), copy.end());

        Log logger;
        std::set<int> std_set(copy.begin(), copy.end());
        auto dur_set = logger.GetDuration();

        Log log;
        Set<int> set(copy.begin(), copy.end());
        auto dur = log.GetDuration();
        std::cout << "ctor sort: " << dur_set << " " << dur << std::endl;

        {
            /* iter */
            Log l_std_set;
            int i = 0;
            for (auto& elem : std_set) {
                REQUIRE(elem == copy[i]);
                i++;
            }
            auto dur_std_set = l_std_set.GetDuration();

            Log l_set;
            i = 0;
            for (auto& elem : set) {
                REQUIRE(elem == copy[i]);
                i++;
            }
            auto dur_set = l_set.GetDuration();
            std::cout << "Iter: " << dur_std_set << " " << dur_set << std::endl;
        }

        {
            /* remove */
            Log l_std_set;
            for (auto elem : copy) {
                std_set.erase(elem);
            }
            auto dur_std_set = l_std_set.GetDuration();

            Log l_set;
            for (auto elem : copy) {
                set.erase(elem);
            }
            auto dur_set = l_set.GetDuration();
            std::cout << "Remove: " << dur_std_set << " " << dur_set << std::endl;
        }
    }
}
