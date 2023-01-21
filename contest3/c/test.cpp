#include <catch.hpp>
#include <main.hpp>

using Requests = std::vector<int>;

// Requests SlowSolution(int size, Requests& requests) {
//     Solution sol(size, requests.size());
//     Requests result;
//     for (auto r : requests) {
//         if (r > 0) {
//             result.push_back(sol.allocate(r));
//         } else {
//             sol.remove(-r);
//         }
//     }
//     return result;
// }

Requests Result(int size, Requests& requests) {
    Manager manager(size, requests.size());
    Requests result;
    for (auto r : requests) {
        if (r > 0) {
            result.push_back(manager.Alloc(r));
        } else {
            manager.Free(-r);
        }
    }
    return result;
}

TEST_CASE("1 пример") {
    int size = 6;
    Requests answer{1, 3, -1, -1, 1, -1};
    Requests r{2, 3, -1, 3, 3, -5, 2, 2};
    REQUIRE(answer == Result(size, r));
}

TEST_CASE("2 пример") {
    int size = 6;
    Requests answer{1, 2, 3, 4, 5, 6};
    Requests r{1,1,1,1,1,1,-1,-2,-3,-4,-5,-6};
    REQUIRE(answer == Result(size, r));
}

TEST_CASE("3 пример") {
    int size = 12;
    Requests answer{
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        1, 3, 5, 7, 9, 11,
        -1, -1, -1,
        9, 5, 1,
        -1, -1, -1, -1};
    Requests r{
        1,1,1,1,1,1,1,1,1,1,1,1,
        -2,-4,-6,-8,-10,-12,
        -1,2,-3,2,-5,2,-7,2,-9,2,-11,2,
        -20,3,-24,3,-28,3,
        -30,4,-26,4,-22,4,
        1,2,3,4};
    REQUIRE(answer == Result(size, r));
}

TEST_CASE("4 пример") {
    int size = 42;
    Requests answer{1, 8, 11, 19, 25, 30, 19};
    Requests r{
        7,
        3,
        8,
        -2,
        6,
        5,
        -5,
        9,
        4
    };
    REQUIRE(answer == Result(size, r));
}

TEST_CASE("5 пример") {
    int size = 128;
    Requests answer{1, 2, 4, 8, 16, 32, 64};
    Requests r{
        1 ,
        2 ,
        4 ,
        -2,
        8 ,
        -3,
        16,
        -5,
        32,
        -7,
        64,
        -1
    };
    REQUIRE(answer == Result(size, r));
}

TEST_CASE("6 пример") {
    int size = 2000000000;
    Requests answer{1, 2000000000, -1, 1, -1, -1};
    Requests r{
        1999999999,
        1,
        2,
        -3,
        -1,
        1999999999,
        1,
        -7,
        1
    };
    REQUIRE(answer == Result(size, r));
}

TEST_CASE("7 пример") {
    int size = 40;
    Requests answer{
        1, 5, 9, 13, 17, 21, 25, 1, 1, 1, 1, 13, 17, 1, 4, 9, 1, 4, 9, 1
    };
    Requests r{
        4, 4, 4, 4, 4, 4,
        -1, -2, 1, -9, -3,
        -4, -5, -6,
        15, -15,
        16, -17,
        10, -19,
        12, 4,
        -21,
        12,
        -22,
        -24,
        3,
        5,
        8,
        -28,
        -27,
        -29,
        3,
        5,
        7,
        -33,
        -35,
        -34,
        16,
        -39,
    };
    REQUIRE(answer == Result(size, r));
}

TEST_CASE("8 пример") {
    int size = 6;
    std::vector<int> answer{1, 3, -1, -1, 1, -1};
    std::vector<int> r{2, 3, -1, 3, 3, -5, 2, 2};
    REQUIRE(answer == Result(size, r));
}

// TEST_CASE("9 пример") {
//     int size = 7;
//     std::vector<int> answer{4,2,4,1,-1,3,2,2,3,-2,4,2};
//     std::vector<int> r{2, 1, 2, 2, -1, 1, 2, 2, 2, -2, 2, 1};
//     REQUIRE(answer == Result(size, r));
// }


TEST_CASE("9 пример") {
    int size = 8;
    std::vector<int> answer{1, 3, 5, 7, -1, 1};
    std::vector<int> r{2, 2, 2, 2, 2, -1, -3, -5, 2};
    REQUIRE(answer == Result(size, r));
}

TEST_CASE("10 пример") {
    std::vector<int> input;
    std::vector<int> baseline;
    std::vector<int> frees;
    int size = 10597;
    for (int i = 1; i < size; i += 2) {
        input.push_back(2);
        baseline.push_back(static_cast<int>(i));
        if (i < size / 2) {
        frees.push_back(-static_cast<int>(i));
        }
    }
    input.push_back(2);
    baseline.push_back(-1);
    std::copy(std::begin(frees), std::end(frees), std::back_inserter(input));
    input.push_back(2);
    baseline.push_back(1);

    REQUIRE(baseline == Result(size, input));
}

TEST_CASE("11 пример") {
    int size = 5;
    std::vector<int> answer{1, -1, 2, 1};
    std::vector<int> r{1, 6, 3, -1, 1};
    REQUIRE(answer == Result(size, r));
}

// void TestSomeInputFromChat() {
//   std::vector<int> input{1, 6, 3, -1, 1};
//   size_t size = 5;

//   auto res = RunDebug(input, size);
//   std::vector<int> baseline{1, -1, 2, 1};

//   if (res != baseline) {
//     PrintVector(res);
//   }

//   assert(res == baseline);
// }

// void TestErrorousFree() {
//   std::vector<int> input{64,  478, 387, 425, -1,  627,
//                          579, 406, 453, -2,  543, 463};
//   size_t size = 1798079;

//   auto res = RunDebug(input, size);
//   std::vector<int> baseline{1,    65,   543,  930,  1355,
//                             1982, 2561, 2967, 3420, 3963};

//   if (res != baseline) {
//     PrintVector(res);
//   }

//   assert(res == baseline);
// }


// void StressTest() {
//   std::uniform_int_distribution<int> dist(1, 3);
//   size_t size = 15;
//   for (int repeats = 0; repeats < 100000; ++repeats) {
//     std::vector<int> input;
//     for (size_t i = 1; i < 21; i += 1) {
//       input.push_back(dist(r_d));
//       if (i % 4 == 0) {
//         input.push_back(-static_cast<int>(i / 4));
//       }
//     }
//     PrintVector(input);
//     auto res = RunDebug(input, size);

//     std::cout << "-------" << std::endl;
//   }
// }
