#include "main.hpp"

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
