#pragma once

#include <random>
#include <vector>
#include <algorithm>
#include <numeric>
#include <filesystem>
#include <chrono>
#include <iostream>

class RandomGenerator {
public:
    explicit RandomGenerator(uint32_t seed = 738547485u) : gen_(seed) {
    }

    template <class T>
    std::vector<T> GenIntegralVector(size_t count, T from, T to) {
        std::uniform_int_distribution<T> dist(from, to);
        std::vector<T> result(count);
        for (auto& cur : result) {
            cur = dist(gen_);
        }
        return result;
    }

    std::string GenString(size_t count, char from = 'a', char to = 'z') {
        std::uniform_int_distribution<char> dist(from, to);
        std::string result(count, from);
        for (char& x : result) {
            x = dist(gen_);
        }
        return result;
    }

    std::vector<double> GenRealVector(size_t count, double from, double to) {
        std::uniform_real_distribution<double> dist(from, to);
        std::vector<double> result(count);
        for (auto& cur : result) {
            cur = dist(gen_);
        }
        return result;
    }

    std::vector<int> GenPermutation(size_t count) {
        std::vector<int> result(count);
        std::iota(result.begin(), result.end(), 0);
        std::shuffle(result.begin(), result.end(), gen_);
        return result;
    }

    template <class T>
    T GenInt(T from, T to) {
        std::uniform_int_distribution<T> dist(from, to);
        return dist(gen_);
    }

    template <class T>
    T GenInt() {
        return GenInt(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }

    template <class Iterator>
    void Shuffle(Iterator first, Iterator last) {
        std::shuffle(first, last, gen_);
    }

private:
    std::mt19937 gen_;
};

inline std::filesystem::path GetFileDir(std::string file) {
    std::filesystem::path path{std::move(file)};
    if (path.is_absolute() && std::filesystem::is_regular_file(path)) {
        return path.parent_path();
    } else {
        throw std::runtime_error{"Bad file name"};
    }
}

class Log {
    std::chrono::high_resolution_clock clock_;
    std::chrono::system_clock::time_point begin_time_;

public:
    Log() {
        begin_time_ = clock_.now();
    }
    int64_t GetDuration() {
        using std::chrono::duration;
        using std::chrono::duration_cast;
        using std::chrono::high_resolution_clock;
        using std::chrono::milliseconds;

        auto end_time = clock_.now();
        auto ms_int = duration_cast<milliseconds>(end_time - begin_time_);
        return ms_int.count();
    }

    int64_t GetDurationNano() {
        using std::chrono::duration;
        using std::chrono::duration_cast;
        using std::chrono::high_resolution_clock;
        using std::chrono::nanoseconds;

        auto end_time = clock_.now();
        auto ms_int = duration_cast<nanoseconds>(end_time - begin_time_);
        return ms_int.count();
    }
};


void Dump(const std::vector<int>& data) {
    for (int i = 0; i < static_cast<int>(data.size()); ++i) {
        std::cout << data[i];
        if (i != static_cast<int>(data.size()) - 1) {
            std::cout << ", ";
        }
    }
}
