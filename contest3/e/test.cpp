#include <main.hpp>
#include <catch.hpp>

TEST_CASE("Пример 1") {
    std::vector<std::vector<int>> triangles = {
        {6, 6, 10},
        {15, 25, 15},
        {35, 21, 21},
    };
    SimilaritiesTriHashTable similarities_tri;
    for (const auto& tri : triangles) {
        similarities_tri.AddTriangle(tri);
    }
    REQUIRE(similarities_tri.GetSimilarities() == 1);
}

TEST_CASE("Пример 2") {
    std::vector<std::vector<int>> triangles = {
        {3, 4, 5},
        {10, 11, 12},
        {6, 7, 8},
        {6, 8, 10},
    };
    SimilaritiesTriHashTable similarities_tri;
    for (const auto& tri : triangles) {
        similarities_tri.AddTriangle(tri);
    }
    REQUIRE(similarities_tri.GetSimilarities() == 3);
}

TEST_CASE("Пример 3") {
    std::vector<std::vector<int>> triangles = {
        {1, 1, 1},
        {10, 11, 12},
    };
    SimilaritiesTriHashTable similarities_tri;
    for (const auto& tri : triangles) {
        similarities_tri.AddTriangle(tri);
    }
    REQUIRE(similarities_tri.GetSimilarities() == 2);
}

TEST_CASE("Пример 4") {
    std::vector<std::vector<int>> triangles = {
        {0, 0, 0},
        {10, 11, 12},
    };
    SimilaritiesTriHashTable similarities_tri;
    for (const auto& tri : triangles) {
        similarities_tri.AddTriangle(tri);
    }
    REQUIRE(similarities_tri.GetSimilarities() == 2);
}

TEST_CASE("Пример 5") {
    std::vector<std::vector<int>> triangles = {
        {-1, 1, 2},
        {1, 1, 2},
    };
    SimilaritiesTriHashTable similarities_tri;
    for (const auto& tri : triangles) {
        similarities_tri.AddTriangle(tri);
    }
    REQUIRE(similarities_tri.GetSimilarities() == 2);
}

TEST_CASE("Пример 6") {
    int big = 1'000'000'000;

    std::vector<std::vector<int>> triangles = {
        {1, big, big},
        {1, big - 1, big - 1},
    };
    SimilaritiesTriHashTable similarities_tri;
    for (const auto& tri : triangles) {
        similarities_tri.AddTriangle(tri);
    }
    REQUIRE(similarities_tri.GetSimilarities() == 2);
}

TEST_CASE("Пример 7") {
    std::vector<std::vector<int>> triangles = {
        {3, 3, 3},
        {4, 4, 4},
    };
    SimilaritiesTriHashTable similarities_tri;
    for (const auto& tri : triangles) {
        similarities_tri.AddTriangle(tri);
    }
    REQUIRE(similarities_tri.GetSimilarities() == 1);
}
