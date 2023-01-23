#include <catch.hpp>
#include <main.hpp>
#include <util.h>
#include <utility>

std::pair<std::vector<int>, int> Erase(std::vector<int> data, int val) {
    if (data.empty()) {
        return {{}, -1};
    }
    auto it = std::lower_bound(data.rbegin(), data.rend(), val);
    int res;
    if (it == data.rend()) {
        res = data.front();
        data.clear();
    } else {
        res = *it;
        data.erase((it + 1).base(), data.end());
    }
    return {data, res};
}

TEST_CASE("Test Erase") {
    std::vector<int> data = {8, 5, 3};

    {
        auto pair = Erase(data, 1);
        REQUIRE(3 == pair.second);
        REQUIRE(std::vector<int>{8, 5} == pair.first);
    }

    {
        auto pair = Erase(data, 3);
        REQUIRE(3 == pair.second);
        REQUIRE(std::vector<int>{8, 5} == pair.first);
    }

    {
        auto pair = Erase(data, 4);
        REQUIRE(5 == pair.second);
        REQUIRE(std::vector<int>{8} == pair.first);
    }

    {
        auto pair = Erase(data, 5);
        REQUIRE(5 == pair.second);
        REQUIRE(std::vector<int>{8} == pair.first);
    }

    {
        auto pair = Erase(data, 7);
        REQUIRE(8 == pair.second);
        REQUIRE(std::vector<int>{} == pair.first);
    }

    {
        auto pair = Erase(data, 8);
        REQUIRE(8 == pair.second);
        REQUIRE(std::vector<int>{} == pair.first);
    }

    {
        auto pair = Erase(data, 10);
        REQUIRE(8 == pair.second);
        REQUIRE(std::vector<int>{} == pair.first);
    }

    {
        auto pair = Erase({}, 10);
        REQUIRE(-1 == pair.second);
        REQUIRE(std::vector<int>{} == pair.first);
    }
}

void TestRecover(const std::vector<int>& data) {
    Bst tree;
    tree.RecoverFromPreorder(data);
    REQUIRE(data == tree.Preorder());
}

TEST_CASE("BST recover") {
    TestRecover({4, 2, 1, 3, 6, 5, 7});
    TestRecover({5, 3, 2, 3, 5, 6});
    TestRecover({1});
    TestRecover({});
    {
    }
    // 5 3 2 3 5 6
}

TEST_CASE("BST recover stress") {
    int size = 100'000;
    {
        std::vector<int> data(size);
        std::iota(data.begin(), data.end(), 0);
        Bst tree;
        Log l;
        tree.RecoverFromPreorder(data);
        std::cout << l.GetDuration() << std::endl;
        REQUIRE(data == tree.Preorder());
    }
    {
        std::vector<int> data(size);
        std::iota(data.rbegin(), data.rend(), 0);
        Bst tree;
        Log l;
        tree.RecoverFromPreorder(data);
        std::cout << l.GetDuration() << std::endl;
        REQUIRE(data == tree.Preorder());
    }
}

TEST_CASE("1 Пример") {
    std::vector<int> preorder{4, 2, 1, 3, 6, 5, 7};
    std::vector<int> postorder{1, 3, 2, 5, 7, 6, 4};
    std::vector<int> inorder{1, 2, 3, 4, 5, 6, 7};
    Bst tree;
    tree.RecoverFromPreorder(preorder);
    auto pair = tree.InPostOrder();
    REQUIRE(inorder == pair.second);
    REQUIRE(postorder == pair.first);
}

TEST_CASE("2 Пример") {
    std::vector<int> preorder{5, 3, 2, 3, 5, 6};
    std::vector<int> postorder{2, 3, 3, 6, 5, 5};
    std::vector<int> inorder{2, 3, 3, 5, 5, 6};
    Bst tree;
    tree.RecoverFromPreorder(preorder);
    auto pair = tree.InPostOrder();
    REQUIRE(inorder == pair.second);
    REQUIRE(postorder == pair.first);
}
