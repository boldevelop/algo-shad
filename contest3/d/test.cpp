#include <main.hpp>
#include <catch.hpp>
#include <util.h>
#include <functional>

std::vector<int> SlowSolution(int k_stat, const std::string& directions, std::vector<int> data) {
    std::vector<int> result;
    auto left_i = 0;
    auto right_i = 0;

    for (char dir : directions) {
        if (dir == 'R') {
            ++right_i;
        } else {
            ++left_i;
        }

        auto begin = data.begin() + left_i;
        auto end = data.begin() + right_i + 1;
        std::vector<int> sorted = {begin, end};
        std::sort(sorted.begin(), sorted.end());

        if (static_cast<int>(sorted.size()) >= k_stat) {
            result.push_back(sorted[k_stat - 1]);
        } else {
            result.push_back(-1);
        }
    }

    return result;
}

void FillHeap(Heap* p) {
    for (int i = 10; i > 0; --i) {
        p->Add({10 - i, i});
        REQUIRE(p->Min() == i);
    }
}

TEST_CASE("Heap Construct from vector") {
    Heap p;
    RandomGenerator rand_gen;
    std::vector<int> iota_v(10);
    std::iota(iota_v.begin(), iota_v.end(), 1);

    std::vector<HeapItem> data;
    for (int i = 0; i < static_cast<int>(iota_v.size()); ++i) {
        data.push_back({i, iota_v[i]});
    }

    int count = 10;
    while (count > 0) {
        rand_gen.Shuffle(data.begin(), data.end());
        Heap p(data);
        REQUIRE(p.Min() == 1);
        REQUIRE(p.MapSize() == 10);

        for (int i = 0; i < 10; ++i) {
            auto min = p.ExtractMin();
            REQUIRE(min == i + 1);
            REQUIRE(p.MapSize() == 10 - (i + 1));
        }
        --count;
    }
}

TEST_CASE("Heap extract min") {
    {
        Heap p;

        for (int i = 1; i <= 10; ++i) {
            p.Add({i - 1, i});
            REQUIRE(p.Min() == 1);
        }

        for (int i = 0; i < 10; ++i) {
            auto min = p.ExtractMin();
            REQUIRE(min == i + 1);
        }
        REQUIRE(p.Empty());

        p.Add({0, 4});
        REQUIRE(p.Min() == 4);

        p.Add({1, 6});
        REQUIRE(p.Min() == 4);

        p.Add({2, 1});
        REQUIRE(p.Min() == 1);
    }

    Heap p;
    FillHeap(&p);

    for (int i = 0; i < 10; ++i) {
        auto min = p.ExtractMin();
        REQUIRE(min == i + 1);
    }
    REQUIRE(p.Empty());
}

// TEST_CASE("Heap remove") {
//     Heap p;
//     FillHeap(&p);

//     for (int i = 0; i < 9; ++i) {
//         p.Remove(1);
//         REQUIRE(p.Min() == 1);
//     }
//     auto min = p.ExtractMin();
//     REQUIRE(min == 1);
//     REQUIRE(p.Empty());
// }

TEST_CASE("Heap k statistics") {
    {
        int k = 2;
        Heap p({{0, 4}});

        p.Add({1, 2});
        REQUIRE(p.GetKStatistic(k) == 4);

        p.Add({3, 1});
        REQUIRE(p.GetKStatistic(k) == 2);

        p.Remove({0, 4});
        REQUIRE(p.GetKStatistic(k) == 2);

        p.Remove({1, 2});
        REQUIRE(p.GetKStatistic(k) == -1);
    }

    {
        int k = 1;
        Heap p({{0, 1}});

        p.Add({1, 2});
        REQUIRE(p.GetKStatistic(k) == 1);

        p.Remove({0, 1});
        REQUIRE(p.GetKStatistic(k) == 2);

        p.Add({2, 3});
        REQUIRE(p.GetKStatistic(k) == 2);

        p.Remove({1, 2});
        REQUIRE(p.GetKStatistic(k) == 3);

        p.Add({3, 4});
        REQUIRE(p.GetKStatistic(k) == 3);

        p.Remove({2, 3});
        REQUIRE(p.GetKStatistic(k) == 4);
    }
}

TEST_CASE("Inplace Head sort") {
    RandomGenerator rand_gen;
    std::vector<int> correct(9);
    std::iota(correct.begin(), correct.end(), 1);
    std::sort(correct.begin(), correct.end(), std::greater<>());

    std::vector<int> initial_val = {7, 4, 2, 6, 1, 5, 3, 8, 9};
    std::vector<HeapItem> data;
    for (int i = 0; i < static_cast<int>(initial_val.size()); ++i) {
        data.push_back({i, initial_val[i]});
    }

    int count = 10;
    while (count > 0) {
        rand_gen.Shuffle(data.begin(), data.end());
        Heap p(data);
        REQUIRE(p.Min() == 1);
        REQUIRE(p.MapSize() == 9);

        p.InplaceSort();

        auto possible_correct = p.GetValueData();
        REQUIRE(possible_correct.size() == correct.size());
        for (int i = 0; i < static_cast<int>(possible_correct.size()); ++i) {
            REQUIRE(possible_correct[i] == correct[i]);
        }

        --count;
    }
}


TEST_CASE("Тестовые примеры") {
    /*
    4 2 1 3 6 5 7
    RRLL
    */
    {
        std::vector<int> data = {4, 2, 1, 3, 6, 5, 7};
        std::string directions = "RRLL";
        int k = 2;

        REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
    }
    /*
    1 2 3 4
    RLRLRL
     */
    {
        std::vector<int> data = {1, 2, 3, 4};
        std::string directions = "RLRLRL";
        int k = 1;

        REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
    }
}



TEST_CASE("Stress small") {
    {
        auto size = 20;
        RandomGenerator rand_gen;
        std::vector<int> data(size);
        std::iota(data.begin(), data.end(), 1);
        rand_gen.Shuffle(data.begin(), data.end());

        std::string directions;
        for (int i = 0; i < size - 1; ++i) {
            directions += "R";
        }
        for (int i = 0; i < size - 1; ++i) {
            directions += "L";
        }

        int k = 1;
        while (k <= size) {
            Log l;
            auto answer = GetKStatistic(k, directions, data);
            auto duration = l.GetDuration();
            if (duration > 500) {
                std::cout << k << ": "<< duration << '\n';
            }
            REQUIRE(SlowSolution(k, directions, data) == answer);
            ++k;
        }
    }

    {
        auto size = 200;
        RandomGenerator rand_gen;
        std::vector<int> data(size);
        std::iota(data.begin(), data.end(), 1);
        rand_gen.Shuffle(data.begin(), data.end());

        int k = 1;
        while (k < size) {
            int r = 0;
            int l = 0;
            std::string directions;
            while (r + k < size && l + 1 < size) {
                if (l + 1 >= size) {
                    directions += std::string(k, 'R');
                    r += k;
                    continue;
                }
                directions += std::string(k, 'R');
                directions += "L";
                l++;
                r += k;
            }

            if (static_cast<int>(directions.size()) > (size - 1) * 2) {
                std::cout << directions.size() << " WRONG\n";
            }

            Log log;
            auto answer = GetKStatistic(k, directions, data);
            auto duration = log.GetDuration();
            if (duration > 500) {
                std::cout << k << ": "<< duration << '\n';
            }
            REQUIRE(SlowSolution(k, directions, data) == answer);
            ++k;
        }
    }
}

TEST_CASE("Stress 10000") {
    auto size = 10000;
    std::cout << "test: " << size << std::endl;
    RandomGenerator rand_gen;
    std::vector<int> data(size);
    std::iota(data.begin(), data.end(), 1);
    rand_gen.Shuffle(data.begin(), data.end());
    std::string directions;
    for (int i = 0; i < size - 1; ++i) {
        directions += "R";
    }
    for (int i = 0; i < size - 1; ++i) {
        directions += "L";
    }
    int k = size / 2 - 1;

    Log l;
    std::cout << "start" << std::endl;
    auto answer = GetKStatistic(k, directions, data);
    auto duration = l.GetDuration();
    if (duration > 500) {
        std::cout << "s: " << size << " k: " << k << " dur: " << duration << std::endl;
    }
    std::cout << "end" << std::endl;
    REQUIRE(SlowSolution(k, directions, data) == answer);
}

TEST_CASE("Stress size") {
    auto size = 10;
    while (size <= 100000) {
        std::cout << "test: " << size << std::endl;
        RandomGenerator rand_gen;
        std::vector<int> data(size);
        std::iota(data.begin(), data.end(), 1);
        rand_gen.Shuffle(data.begin(), data.end());
        std::string directions;
        for (int i = 0; i < size - 1; ++i) {
            directions += "R";
        }
        for (int i = 0; i < size - 1; ++i) {
            directions += "L";
        }
        int k = size / 2 - 1;

        Log l;
        std::cout << "start" << std::endl;
        auto answer = GetKStatistic(k, directions, data);
        auto duration = l.GetDuration();
        if (duration > 500) {
            std::cout << "s: " << size << " k: " << k << " dur: " << duration << std::endl;
        }
        std::cout << "end" << std::endl;
        REQUIRE(SlowSolution(k, directions, data) == answer);
        size *= 10;
    }

    // {
    //     auto size = 1000;
    //     RandomGenerator rand_gen;
    //     std::vector<int> data(size);
    //     std::iota(data.begin(), data.end(), 1);
    //     std::string directions;
    //     for (int i = 0; i < size - 1; ++i) {
    //         directions += "R";
    //     }
    //     for (int i = 0; i < size - 1; ++i) {
    //         directions += "L";
    //     }

    //     int k = 1;
    //     while (k <= size) {
    //         Log l;
    //         auto answer = GetKStatistic(k, directions, data);
    //         auto duration = l.GetDuration();
    //         if (duration > 500) {
    //             std::cout << k << ": "<< duration << '\n';
    //         }
    //         REQUIRE(SlowSolution(k, directions, data) == answer);
    //         k += 10000;
    //     }
    // }

    // {
    //     RandomGenerator rand_gen;
    //     std::vector<int> data(size);
    //     std::iota(data.begin(), data.end(), 1);
    //     std::string directions;
    //     for (int i = 0; i < size - 1; ++i) {
    //         directions += "R";
    //     }
    //     for (int i = 0; i < size - 1; ++i) {
    //         directions += "L";
    //     }

    //     int k = 5;
    //     REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
    // }
}
