#pragma once

#include <vector>
#include <random>
#include <optional>
#include <iostream>
#include <string>

std::mt19937 gen(738547485u);
const uint64_t kPrime = 2 * 1'000'000'000 + 11;

struct Hash {
    uint64_t a_term;
    uint64_t b_term;

    Hash() : a_term(0), b_term(0) {
    }

    uint64_t operator()(uint64_t x) const {
        return (a_term * x + b_term) % kPrime;
    }

    void GenerateAB() {
        a_term = gen();
        b_term = gen();
    }
};

class Bucket {
    Hash hash_;
    std::vector<std::optional<int>> data_;

public:
    Bucket() : hash_(), data_() {
    }

    void Init(const std::vector<int> numbers) {
        auto size = numbers.size() * numbers.size();

        auto is_repeat = true;
        while (is_repeat) {
            data_.clear();
            data_.resize(size);
            hash_.GenerateAB();
            auto collision = false;
            for (auto num : numbers) {
                auto& val = data_[hash_(num) % size];
                if (val.has_value()) {
                    collision = true;
                    break;
                }
                val = num;
            }
            is_repeat = collision;
        }
    }

    bool Contains(int number) const {
        if (data_.size() == 0) {
            return false;
        }
        auto val = data_[hash_(number) % data_.size()];
        return val.has_value() && number == *val;
    }

    void Print() const {
        auto is_first = true;
        for (auto num_opt : data_) {
            if (!is_first) {
                std::cout << ", ";
            }
            std::cout << (num_opt.has_value() ? std::to_string(*num_opt) : "*");
            is_first = false;
        }
    }
};

class FixedSet {
    Hash hash_;
    std::vector<Bucket> buckets_;

public:
    FixedSet() : hash_(), buckets_() {
    }

    void Initialize(const std::vector<int>& numbers) {
        buckets_.clear();
        hash_.GenerateAB();

        auto size = numbers.size() > 1 ? numbers.size() * 2 / 3 : 1;
        buckets_.resize(size);

        std::vector<std::vector<int>> bucket_numbers;
        bucket_numbers.resize(size);

        for (auto num : numbers) {
            auto index = hash_(num) % size;
            bucket_numbers[index].push_back(num);
        }

        for (size_t i = 0; i < bucket_numbers.size(); ++i) {
            buckets_[i].Init(bucket_numbers[i]);
        }
    }

    bool Contains(int number) const {
        return buckets_[hash_(number) % buckets_.size()].Contains(number);
    }

    void Print() const {
        auto i = 0;
        for (auto& buck : buckets_) {
            std::cout << "[" << i << "] - [";
            buck.Print();
            ++i;
            std::cout << "]\n";
        }
        std::cout << "\n";
    }
};
