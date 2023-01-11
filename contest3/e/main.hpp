#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <list>

std::mt19937 gen(738547485u);

struct Hash {
    uint64_t kPrime = 2 * 1'000'000'000 + 11;

    uint64_t operator()(const std::vector<int>& triangle) {
        auto a_term = static_cast<uint64_t>(triangle[0]) * kPrime * kPrime;
        auto b_term = static_cast<uint64_t>(triangle[1]) * kPrime;
        auto c_term = static_cast<uint64_t>(triangle[2]);
        return a_term + b_term + c_term;
    }
};

class SimilaritiesTriHashTable {
    std::vector<std::vector<std::vector<int>>> data_;
    Hash hash_;
    int similarities_;

public:
    SimilaritiesTriHashTable() : data_(50000), hash_(), similarities_(0) {
    }

    void AddTriangle(const std::vector<int>& triangle) {
        auto tri = triangle;
        NormalizeTriangle(&tri);
        auto& count = data_[hash_(tri) % data_.size()];
        if (count.empty()) {
            count.push_back(tri);
            similarities_++;
        } else {
            for (auto stored_tri : count) {
                // if (static_cast<int64_t>(stored_tri[0]) * static_cast<int64_t>(tri[1]) !=
                //         static_cast<int64_t>(stored_tri[1]) * static_cast<int64_t>(tri[0]) ||
                //     static_cast<int64_t>(stored_tri[0]) * static_cast<int64_t>(tri[2]) !=
                //         static_cast<int64_t>(stored_tri[2]) * static_cast<int64_t>(tri[0])) {
                //     count.push_back(tri);
                //     similarities_++;
                //     break;
                // }

                if (stored_tri[0] != tri[0] || stored_tri[1] != tri[1] || stored_tri[2] != tri[2]) {
                    count.push_back(tri);
                    similarities_++;
                    break;
                }
            }
        }
    }

    int GetSimilarities() const {
        return similarities_;
    }

private:
    void NormalizeTriangle(std::vector<int>* tri) {
        std::sort(tri->begin(), tri->end());
        auto gcd_term = std::gcd((*tri)[0], std::gcd((*tri)[1], (*tri)[2]));
        if (gcd_term != 0) {
            for (auto& num : *tri) {
                num /= gcd_term;
            }
        }
    }
};

int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    int n_term;
    std::vector<int> triangle;
    SimilaritiesTriHashTable similarities_tri;
    std::cin >> n_term;
    while (n_term > 0) {
        int side;
        for (int i = 0; i < 3; ++i) {
            std::cin >> side;
            triangle.push_back(side);
        }
        similarities_tri.AddTriangle(triangle);
        triangle.clear();
        --n_term;
    }
    std::cout << similarities_tri.GetSimilarities() << std::endl;
}
