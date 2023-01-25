#include <main.hpp>
#include <catch.hpp>
#include <util.h>

/*
1 1,
2 2,
3 5,
4 14,
5 42,
6 132,
7 429,
8 1430,
9 4862,
10 16796,
11 58786,
12 208012,
13 742900,
14 2674440,
15 9694845,
16 35357670,
17 129644790,
18 477638700,
19 1'767'263'190,
20 6'564'120'420,
21 24466267020,
22 91482563640,
23 343059613650,
24 1289904147324,
25 4861946401452,
*/

class Catalan {
    Seq catalan_;

public:
    Catalan(int max_num) : catalan_(max_num + 1) {
        GenerateCatalanNumbers();
    }
    void Print() {
        int num = 1;
        while (num < static_cast<int>(catalan_.size()) - 1) {
            std::cout << num << " " << GetCatalanNum(num) << std::endl;
            ++num;
        }
    }
    int GetCatalanNum(int num) {
        return catalan_[num + 1];
    }
private:
    void GenerateCatalanNumbers() {
        catalan_[1] = 1;
        for (int i = 2; i < static_cast<int>(catalan_.size()); i++) {
            for (int j = 1; j <= i - 1; j++) {
                catalan_[i] = (catalan_[i] + ((catalan_[j]) * catalan_[i - j]) % mod) % mod;
            }
            catalan_[i] = catalan_[i] % mod;
        }
    }
};

TEST_CASE("Пример 0") {
    REQUIRE(CountBST({1, 1, 1}) == 1);
    REQUIRE(CountBST({1, 2, 2}) == 2);
    REQUIRE(CountBST({1, 1, 2}) == 3);
}

TEST_CASE("Пример 1") {
    Seq data{2, 1};
    REQUIRE(CountBST(data) == 2);
}

TEST_CASE("Пример 2") {
    Seq data{10, 10, 10};
    REQUIRE(CountBST(data) == 1);
}

TEST_CASE("Пример 3") {
    Seq data{1, 2, 3};
    REQUIRE(CountBST(data) == 5);
}

TEST_CASE("Пример 3.1") {
    Seq data{1, 1, 2, 2, 3, 3, 4, 4};
    REQUIRE(CountBST(data) == 55);
}

TEST_CASE("Пример 3.2") {
    Seq data{1, 1, 2};
    REQUIRE(CountBST(data) == 3);
}

TEST_CASE("Пример 3.3") {
    Seq data{1, 2, 2};
    REQUIRE(CountBST(data) == 2);
}

TEST_CASE("Пример 4") {
    int size = 1;
    Catalan catalan(401);
    while (size < 401) {
        Seq data(size);
        std::iota(data.begin(), data.end(), 1);
        Log l;
        auto res = CountBST(data);
        auto dur = l.GetDuration();
        if (dur > 500) {
            std::cout << "s: " << size << " ";
            std::cout << l.GetDuration() << std::endl;
        }
        REQUIRE(res == catalan.GetCatalanNum(size));
        size++;
    }

    size = 1;
    while (size < 401) {
        Seq data(size, -100);
        Log l;
        auto res = CountBST(data);
        auto dur = l.GetDuration();
        if (dur > 500) {
            std::cout << "s: " << size << " ";
            std::cout << l.GetDuration() << std::endl;
        }
        REQUIRE(res == 1);
        size++;
    }
}

TEST_CASE("Пример 5") {
    Seq data{1, 2, 2, 3};
    REQUIRE(CountBST(data) == 7); // ?
}
