#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using Saved = std::vector<std::vector<double>>;

void CalcProbabilityTreapHeight(int size, int height, Saved* saved) {
    /* n = height, k = size  */
    for (int left = 0; left < size; ++left) {
        int right = size - (left + 1);
        double l_subtree = left == 0 ? 1.0 : (*saved)[height - 1][left];
        double r_subtree = right == 0 ? 1.0 : (*saved)[height - 1][right];
        (*saved)[height][size] += l_subtree * r_subtree;
    }
    (*saved)[height][size] /= size;
}

#ifndef LOCAL
int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    int size;
    int height;
    std::cin >> size >> height;

    if (height >= size) {
        std::cout << 0 << '\n';
        return 0;
    }

    Saved saved(size + 1, std::vector<double>(size + 1, 0.));

    for (int i = 1; i < size + 1; ++i) {
        saved[i][0] = 1.;
        saved[i][1] = 1.;
    }

    for (int i = 2; i < size + 1; ++i) {      // size
        for (int j = 2; j < size + 1; ++j) {  // height
            CalcProbabilityTreapHeight(i, j, &saved);
        }
    }

    std::cout << std::fixed << std::setprecision(10)
              << saved[height + 1][size] - saved[height][size] << '\n';

    return 0;
}

#endif
