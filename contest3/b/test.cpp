#include <catch.hpp>
#include <main.hpp>

TEST_CASE("Пример 1") {
    Tree first(4);
    Tree second(4);

    first.AddEdge(3, 1);
    first.AddEdge(2, 3);
    first.AddEdge(4, 3);

    second.AddEdge(2, 4);
    second.AddEdge(4, 1);
    second.AddEdge(3, 4);
    std::cout << "Should 1 2 4 3: \n";
    REQUIRE(first.IsIsomorphicWith(second));
}

TEST_CASE("Пример 2") {
    Tree first(4);
    Tree second(4);

    first.AddEdge(1, 2);
    first.AddEdge(2, 3);
    first.AddEdge(3, 4);

    second.AddEdge(2, 1);
    second.AddEdge(1, 3);
    second.AddEdge(4, 1);
    std::cout << "Should -1: ";
    REQUIRE(!first.IsIsomorphicWith(second));
}

TEST_CASE("Пример 3") {
    Tree first(4);
    Tree second(4);

    first.AddEdge(1, 2);
    first.AddEdge(2, 3);
    first.AddEdge(3, 4);

    second.AddEdge(3, 4);
    second.AddEdge(1, 4);
    second.AddEdge(2, 1);

    std::cout << "Should 3 4 1 2 (?) \n";
    REQUIRE(first.IsIsomorphicWith(second));
}

TEST_CASE("Пример 4") {
    Tree first(4);
    Tree second(4);

    first.AddEdge(1, 2);
    first.AddEdge(2, 3);
    first.AddEdge(3, 4);

    second.AddEdge(3, 4);
    second.AddEdge(1, 4);
    second.AddEdge(2, 1);

    std::cout << "Should 3 4 1 2 (?) \n";
    REQUIRE(first.IsIsomorphicWith(second));
}
