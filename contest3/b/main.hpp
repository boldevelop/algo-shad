#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <stack>

struct HashData {
    long double hash;
    int vertex;
};

const uint64_t kPrime = 2 * 1'000'000'000 + 11;

class VertexHashTable {
    std::vector<std::list<HashData>> data_;
    int size_;

public:
    VertexHashTable() : data_(0) {
    }

    void Resize(int need_capacity) {
        if (need_capacity < 5000) {
            data_.resize(need_capacity);
        } else {
            data_.resize(5000);
        }
    }

    void Add(long double key, int vertex) {
        data_[Hash(key) % 10000].push_back({key, vertex});
        size_++;
    }

    int Remove(long double key) {
        auto& list = data_[Hash(key) % 10000];
        int vertex = -1;

        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it->hash == key) {
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
    }

private:
    uint Hash(long double key) const {
        uint hash = 0;
        std::memcpy(&key, &hash, sizeof(hash));
        return hash % kPrime;
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

    long double Hash(int centroid) {
        hashes_.resize(size_);
        auto hash = DfsHash(centroid, -1, nullptr);
        return hash;
    }

    long double HashWithMap(int centroid, std::vector<VertexHashTable>* map) {
        auto hash = DfsHash(centroid, -1, map);
        return hash;
    }

    void Print(int vertex) {
        DfsPrint(vertex, -1, "");
    }

    bool IsIsomorphicWith(Tree& tree) {
        auto centroids = GetCentroids();
        auto another_centroids = tree.GetCentroids();
        if (centroids.size() != another_centroids.size()) {
            // std::cout << "-1" << std::endl;
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

        // std::cout << "-1" << std::endl;

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
        // std::cout << '\n';
        // std::cout << "new centroid: " << c_new << std::endl;
        // for (int i = 0; i < size_; ++i) {
        //     std::cout << i << "\n";
        //     for (auto& data : tree.hashes_[i]) {
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
                auto next_vertex = map[vertexes.old_v].Remove(data.hash);
                stack_vert.push({next_vertex, data.vertex});
            }
        }

        // PrintOldKeys(c_old, c_new, map, &old_new_vertex);
        // for (auto new_vert : old_new_vertex) {
        //     std::cout << (new_vert + 1) << "\n";
        // }
    }

    void PrintOldKeys(int old_v, int new_v, std::vector<VertexHashTable>& map,
                      std::vector<int>* old_new_vertex) {
        (*old_new_vertex)[old_v] = new_v;

        const auto& new_child = hashes_[new_v];
        for (auto& data : new_child) {
            auto next_vertex = map[old_v].Remove(data.hash);
            PrintOldKeys(next_vertex, data.vertex, map, old_new_vertex);
        }
    }

private:
    long double DfsHash(int vertex, int prev_vert, std::vector<VertexHashTable>* map) {
        bool is_leaf = true;

        std::vector<HashData> child_hashes;

        for (int child_v : data_[vertex]) {
            if (child_v != prev_vert) {
                is_leaf = false;
                child_hashes.push_back({std::log(DfsHash(child_v, vertex, map)), child_v});
            }
        }

        if (is_leaf) {
            return 199.0;
        }

        std::sort(child_hashes.begin(), child_hashes.end(),
                  [](const HashData& lhs, const HashData& rhs) { return lhs.hash < rhs.hash; });

        long double sum = 3.0;
        for (auto& child_h : child_hashes) {
            sum += child_h.hash;
        }

        if (map == nullptr) {
            hashes_[vertex] = std::move(child_hashes);
        } else {
            auto& map_ref = (*map)[vertex];
            map_ref.Resize(child_hashes.size());
            for (auto& data : child_hashes) {
                map_ref.Add(data.hash, data.vertex);
            }
        }

        return sum;
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

// int main() {
//     std::cin.tie(nullptr);
//     std::ios_base::sync_with_stdio(false);

//     int size;
//     int a_src, b_dist;
//     std::cin >> size;

//     Tree first(size);
//     for (int i = 1; i < size; ++i) {
//         std::cin >> a_src >> b_dist;
//         first.AddEdge(a_src, b_dist);
//     }

//     Tree second(size);
//     for (int i = 1; i < size; ++i) {
//         std::cin >> a_src >> b_dist;
//         second.AddEdge(a_src, b_dist);
//     }

//     first.IsIsomorphicWith(second);
// }

// int main() {
//     std::cin.tie(nullptr);
//     std::ios_base::sync_with_stdio(false);

//     int t;
//     std::cin >> t;
//     while (t > 0) {
//         int size;
//         int a, b;
//         std::cin >> size;

//         Tree first(size);
//         for (int i = 1; i < size; ++i) {
//             std::cin >> a >> b;
//             first.AddEdge(a, b);
//         }

//         Tree second(size);
//         for (int i = 1; i < size; ++i) {
//             std::cin >> a >> b;
//             second.AddEdge(a, b);
//         }

//         if (first.IsIsomorphicWith(second)) {
//             std::cout << "YES" << '\n';
//         } else {
//             std::cout << "NO" << '\n';
//         }

//         --t;
//     }
// }
