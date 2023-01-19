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

TEST_CASE("Тестовые примеры") {
    /*
    4 2 1 3 6 5 7
    RRLL
    */
    {
        std::cout << ((0-1)/2) << std::endl;
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

    {
        std::vector<int> data = {1};
        std::string directions = "";
        int k = 1;

        REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
    }
    /*
    4 6 1
    1 2 3 4
    RLRRLL
    */
    {
        std::vector<int> data = {1, 2, 3, 4};
        std::string directions = "RLRRLL";
        int k = 1;

        REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
    }
    /*
    4 2 1 3 6 5 7
    RRLL
    */
    {
        std::vector<int> data = {4, 2, 1, 3, 6, 5, 7};
        std::string directions = "RRRRLLLLRRLL";
        int k = 7;

        REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
    }
    /*
    4 2 1 3 6 5 7
    RRLL
    */
    {
        std::vector<int> data = {4, 2, 1, 3, 6, 5, 7};
        std::string directions = "RL";
        int k = 1;

        REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
    }
    /*
    4 2 1 3 6 5 7
    RRLL
    */
    {
        std::vector<int> data = {4, 2, 1, 3, 6, 5, 7};
        std::string directions = "RRLRLLRLRRLL";
        int k = 4;

        REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
    }
    /*
    4 2 1 3 6 5 7
    RRLL
    */
    {
        std::vector<int> data = {0, 0, 0, 0, 0, 0, 0};
        std::string directions = "RRLRLLRLRRLL";
        int k = 4;

        REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
    }
    /*
    4 2 1 3 6 5 7
    RRLL
    */
    {
        std::vector<int> data = {9, 8, 7, 6, 5, 4, 3};
        std::string directions = "RRLRLLRLRRLL";
        int k = 4;

        REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
    }
}

TEST_CASE("Another") {
    {
        std::vector<int> input{1, 1, 2, 3};
        std::string moves = "RRR";
        int kstats = 3;
        REQUIRE(SlowSolution(kstats, moves, input) == GetKStatistic(kstats, moves, input));
    }
    {
        std::vector<int> input{2, 0, 4, 6};
        std::string moves = "RRR";
        int kstats = 3;
        REQUIRE(SlowSolution(kstats, moves, input) == GetKStatistic(kstats, moves, input));
    }
    {
        std::vector<int> input{8, 0, 1};
        std::string moves = "RLR";
        int kstats = 3;
        REQUIRE(SlowSolution(kstats, moves, input) == GetKStatistic(kstats, moves, input));
    }
    {
        std::vector<int> input{4, 7, 6};
        std::string moves = "RLR";
        int kstats = 2;
        REQUIRE(SlowSolution(kstats, moves, input) == GetKStatistic(kstats, moves, input));
    }
    {
        std::vector<int> input{8, 5, 8, 1};
        std::string moves = "RRLR";
        int kstats = 3;
        REQUIRE(SlowSolution(kstats, moves, input) == GetKStatistic(kstats, moves, input));
    }
    {
        std::vector<int> input{0, 5, 1, 1, 1};
        std::string moves = "RRRLLR";
        int kstats = 2;
        REQUIRE(SlowSolution(kstats, moves, input) == GetKStatistic(kstats, moves, input));
    }
    {
        std::vector<int> input{5, 6, 6, 7};
        std::string moves = "RRR";
        int kstats = 3;
        REQUIRE(SlowSolution(kstats, moves, input) == GetKStatistic(kstats, moves, input));
    }
    {
        std::vector<int> input{1, 1};
        std::string moves = "RL";
        int kstats = 1;
        REQUIRE(SlowSolution(kstats, moves, input) == GetKStatistic(kstats, moves, input));
    }
    {
        std::vector<int> input{1, 1, 1, 1};
        std::string moves = "R";
        int kstats = 1;
        REQUIRE(SlowSolution(kstats, moves, input) == GetKStatistic(kstats, moves, input));
    }
    {
        std::vector<int> input{1, 1, 1, 1};
        std::string moves = "RR";
        int kstats = 2;
        REQUIRE(SlowSolution(kstats, moves, input) == GetKStatistic(kstats, moves, input));
    }
    {
        std::vector<int> input{1, 1, 1, 1};
        std::string moves = "RLL";
        int kstats = 2;
        REQUIRE(SlowSolution(kstats, moves, input) == GetKStatistic(kstats, moves, input));
    }
}

TEST_CASE("Small 4") {
    RandomGenerator rand_gen;
    int k = 1;
    int size = 5;

    while (k < size) {

        std::vector<int> data(size);
        std::iota(data.begin(), data.end(), 1);
        do {

            int c = 0;
            while (c < 3) {

                if (c == 0) {
                    std::string directions = "RRRR";
                    REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
                } else if (c == 1) {
                    std::string str = "RRL";
                    do {

                        std::string str_next = "RLL";
                        do {
                            std::string directions = "R" + str + str_next + "L";
                            REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
                        } while (std::next_permutation(str_next.begin(), str_next.end()));

                    } while (std::next_permutation(str.begin(), str.end()));
                } else if (c == 2) {
                    std::string str = "RLL";
                    do {

                        std::string str_next = "RRL";
                        do {
                            std::string directions = "R" + str + str_next + "L";
                            REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
                        } while (std::next_permutation(str_next.begin(), str_next.end()));

                    } while (std::next_permutation(str.begin(), str.end()));
                }

                ++c;
            }
        } while (std::next_permutation(data.begin(), data.end()));
        ++k;
    }

}

TEST_CASE("Small") {
    {
        int size = 1;
        RandomGenerator rand_gen;

        while (size < 8) {
            int k = 1;
            while (k < size) {

                std::vector<int> data(size);
                std::iota(data.begin(), data.end(), 1);

                do {
                    int var = 0;
                    while (var < 4) {
                        std::string directions;

                        if (var == 0) {
                            int c = 1;
                            int sum = 0;
                            sum += c * 2;
                            while (sum < size - 1) {
                                directions += std::string(c, 'R');
                                directions += std::string(c, 'L');
                                sum += c * 2;
                                c++;
                            }
                        } else if (var == 1) {
                            directions += std::string(size - 1, 'R');
                        } else if (var == 2) {
                            for (int i = 0; i < size - 1; i = i + 1 + 1) {
                                directions += "RL";
                            }
                        } else if (var == 3) {
                            if (size == 2 || size == 4) {
                                directions += "R";
                            }
                            if (size == 3) {
                                directions += "RL";
                            }
                            // if (size == 4) {
                            //     // RLRRRLL неправильный тест
                            //     std::string s = "RL";
                            //     rand_gen.Shuffle(s.begin(), s.end());
                            //     directions += s;
                            //     directions += "R";
                            //     rand_gen.Shuffle(s.begin(), s.end());
                            //     directions += s;
                            // }
                            // if (size == 6) {

                            // }
                        }

                        // std::cout << size << " " << directions.size() << " " << k << std::endl;
                        // for (auto d : data) {
                        //     std::cout << d << " ";
                        // }
                        // std::cout << std::endl;
                        // std::cout << directions << std::endl;

                        REQUIRE(SlowSolution(k, directions, data) == GetKStatistic(k, directions, data));
                        ++var;
                    }
                }
                while (std::next_permutation(data.begin(), data.end()));

                ++k;
            }

            ++size;
        }
    }
}


TEST_CASE("Stress small") {
    {
        auto size = 4;
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
    while (size <= 10000) {
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
