#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <stack>
#include <limits>

using HashType = uint64_t;

struct HashData {
    HashType hash;
    int vertex;
};

const uint64_t kPrime = 2 * 1'000'000'000 + 11;

class VertexHashTable {
    std::vector<std::list<HashData>> data_;
    int size_ = 0;
    int capacity_ = 0;

public:
    VertexHashTable() : data_(0) {
    }

    void Resize(int need_capacity) {
        if (need_capacity < 5000) {
            capacity_ = need_capacity > 0 ? need_capacity : 1;
        } else {
            capacity_ = 5000;
        }
        data_.resize(capacity_);
    }

    void Add(const HashData& data) {
        auto key = data.hash;
        data_[Hash(key) % capacity_].push_back(data);
        size_++;
    }

    int Remove(const HashData& data) {
        auto key = data.hash;
        auto& list = data_[Hash(key) % capacity_];
        int vertex = -1;

        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it->hash == data.hash) {
                vertex = it->vertex;
                list.erase(it);
                break;
            }
        }

        --size_;
        return vertex;
    }

    bool Empty() const {
        return size_ == 0;
    }

    void Clear() {
        data_.clear();
        size_ = 0;
        capacity_ = 0;
    }

private:
    HashType Hash(HashType key) const {
        return key % kPrime;
    }
};

class Tree {
    std::vector<std::vector<int>> data_;
    int size_;
    std::vector<std::vector<HashData>> hashes_;

public:
    explicit Tree(int size) : data_(size), size_(size) {
    }

    void AddEdge(int src, int dst) {
        data_[src - 1].push_back(dst - 1);
        data_[dst - 1].push_back(src - 1);
    }

    std::vector<int> GetCentroids() {
        std::vector<int> centroids;
        std::vector<int> used(size_);
        DfsGetCentroids(0, -1, used, &centroids);
        return centroids;
    }

    HashType Hash(int centroid) {
        hashes_.resize(size_);
        auto hash = DfsHash(centroid, -1, 1, nullptr);
        return hash;
    }

    HashType HashWithMap(int centroid, std::vector<VertexHashTable>* map) {
        auto hash = DfsHash(centroid, -1, 1, map);
        return hash;
    }

    void Print(int vertex) {
        DfsPrint(vertex, -1, "");
    }

    bool IsIsomorphicWith(Tree& tree) {
        auto centroids = GetCentroids();
        auto another_centroids = tree.GetCentroids();
        if (centroids.size() != another_centroids.size()) {
            std::cout << "-1" << std::endl;
            return false;
        }

        std::vector<VertexHashTable> map(size_);
        auto first_hash = HashWithMap(centroids[0], &map);
        auto is_poly = first_hash == tree.Hash(another_centroids[0]);

        if (is_poly) {
            tree.PrintOldKeys(centroids[0], another_centroids[0], map);
            return true;
        }

        if (centroids.size() == 2 && first_hash == tree.Hash(another_centroids[1])) {
            tree.PrintOldKeys(centroids[0], another_centroids[1], map);
            return true;
        }

        std::cout << "-1" << std::endl;

        return false;
    }

    void PrintOldKeys(int c_old, int c_new, std::vector<VertexHashTable>& map) {
        // std::cout << "centroid: " << c_old << std::endl;
        // for (int i = 0; i < size_; ++i) {
        //     std::cout << i << "\n";
        //     for (auto& data : hashes_[i]) {
        //         std::cout << "*- " << data.vertex << ": " << data.hash << std::endl;
        //     }
        // }

        struct StackData {
            int old_v;
            int new_v;
        };

        std::vector<int> old_new_vertex(size_);
        std::stack<StackData> stack_vert;
        stack_vert.push({c_old, c_new});

        while (!stack_vert.empty()) {
            auto vertexes = stack_vert.top();
            stack_vert.pop();

            old_new_vertex[vertexes.old_v] = vertexes.new_v;

            for (auto& data : hashes_[vertexes.new_v]) {
                auto next_vertex = map[vertexes.old_v].Remove(data);
                stack_vert.push({next_vertex, data.vertex});
            }
        }

        for (auto new_vert : old_new_vertex) {
            std::cout << (new_vert + 1) << "\n";
        }
    }

private:
    HashType DfsHash(int vertex, int prev_vert, uint64_t depth, std::vector<VertexHashTable>* map) {
        bool is_leaf = true;
        std::vector<HashData> child_hashes;

        for (int child_v : data_[vertex]) {
            if (child_v != prev_vert) {
                is_leaf = false;
                auto hash = DfsHash(child_v, vertex, depth + 1, map);
                child_hashes.push_back({hash, child_v});
            }
        }

        if (is_leaf) {
            return depth;
        }

        std::sort(child_hashes.begin(), child_hashes.end(),
                  [](const HashData& lhs, const HashData& rhs) { return lhs.hash < rhs.hash; });

        uint64_t hash = 0;
        uint64_t max_mod = std::numeric_limits<uint64_t>::max() / 2;
        for (auto& child_h : child_hashes) {
            hash =
                hash + (child_h.hash * child_h.hash + child_h.hash * kPrime * depth + 42) % max_mod;
            hash = hash % max_mod;
        }

        if (map == nullptr) {
            hashes_[vertex] = std::move(child_hashes);
        } else {
            auto& map_ref = (*map)[vertex];
            map_ref.Resize(child_hashes.size());
            for (auto& data : child_hashes) {
                map_ref.Add(data);
            }
        }

        return hash;
    }

    void DfsGetCentroids(int vertex, int prev_vert, std::vector<int>& used,
                         std::vector<int>* centroids) {
        used[vertex] = 1;
        bool is_centroid = true;

        for (auto child_v : data_[vertex]) {
            if (child_v != prev_vert) {
                DfsGetCentroids(child_v, vertex, used, centroids);
                used[vertex] += used[child_v];

                if (used[child_v] > size_ / 2) {
                    is_centroid = false;
                }
            }
        }

        if (size_ - used[vertex] > size_ / 2) {
            is_centroid = false;
        }

        if (is_centroid) {
            centroids->push_back(vertex);
        }
    }

    void DfsPrint(int vertex, int prev_vert, std::string indent = "") {
        std::cout << indent << vertex << std::endl;
        for (int child_v : data_[vertex]) {
            if (child_v != prev_vert) {
                DfsPrint(child_v, vertex, indent + "+- ");
            }
        }
    }
};

int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    int size;
    int a_src, b_dist;
    std::cin >> size;

    Tree first(size);
    for (int i = 1; i < size; ++i) {
        std::cin >> a_src >> b_dist;
        first.AddEdge(a_src, b_dist);
    }

    Tree second(size);
    for (int i = 1; i < size; ++i) {
        std::cin >> a_src >> b_dist;
        second.AddEdge(a_src, b_dist);
    }

    first.IsIsomorphicWith(second);
}
