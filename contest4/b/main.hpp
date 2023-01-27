#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <random>
#include <numeric>
#include <limits>
#include <algorithm>

std::mt19937 gen(738547485u);

int GenRandomInt() {
    std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(),
                                            std::numeric_limits<int>::max());
    return dist(gen);
}

namespace simple {
class Treap {
    struct Node {
        int key;
        int prior;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        explicit Node(int added_key)
            : key(added_key), prior(GenRandomInt()), left(nullptr), right(nullptr) {
        }
    };

    using NodePtr = std::shared_ptr<Node>;
    NodePtr root_;

public:
    Treap() : root_(nullptr) {
    }

    void Insert(int val) {
        auto splitted = Split(root_, val);
        NodePtr new_node = std::make_shared<Node>(val);
        root_ = Merge(Merge(splitted.left, new_node), splitted.right);
    }

    void Remove(int val) {
        auto splitted = Split(root_, val);
        root_ = Merge(splitted.left, splitted.right);
    }

    NodePtr Find(int val) {
        auto splitted = Split(root_, val);
        root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
        return splitted.elem;
    }

    NodePtr UpperBound(int val) {
        auto splitted = Split(root_, val);
        root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
        return splitted.right;
    }

    void Print() const {
        PrintImpl(root_);
    }

    int GetHeight() const {
        return GetHeightImpl(root_);
    }
    void PrintData(NodePtr node = nullptr) const {
        PrintDataImpl(node ? node : root_);
        std::cout << std::endl;
    }

private:
    struct Splitted {
        NodePtr left;
        NodePtr elem;
        NodePtr right;
    };

    NodePtr Merge(NodePtr left, NodePtr right) {
        if (!left || !right) {
            return left ? left : right;
        }
        if (left->prior > right->prior) {
            left->right = Merge(left->right, right);
            return left;
        }
        right->left = Merge(left, right->left);
        return right;
    }

    Splitted Split(NodePtr node, int val) {
        if (!node) {
            return {nullptr, nullptr, nullptr};
        }
        if (node->key == val) {
            Splitted splitted{node->left, node, node->right};
            splitted.elem->left = nullptr;
            splitted.elem->right = nullptr;
            return splitted;
        }
        if (node->key < val) {
            auto splitted = Split(node->right, val);
            node->right = splitted.left;
            return {node, splitted.elem, splitted.right};
        }
        auto splitted = Split(node->left, val);
        node->left = splitted.right;
        return {splitted.left, splitted.elem, node};
    }

    void PrintImpl(NodePtr node, std::string indent = "") const {
        if (!node) {
            return;
        }
        if (node == root_) {
            std::cout << "Treap\n";
        }
        std::cout << indent << node->key << " (" << node->prior << ')' << '\n';
        indent = indent.empty() ? "'-> " : "    " + indent;
        PrintImpl(node->left, indent);
        PrintImpl(node->right, indent);
    }
    void PrintDataImpl(NodePtr node) const {
        if (!node) {
            return;
        }
        PrintDataImpl(node->left);
        std::cout << node->key << " ";
        PrintDataImpl(node->right);
    }
    int GetHeightImpl(NodePtr node) const {
        if (!node) {
            return 0;
        }
        auto left = GetHeightImpl(node->left);
        auto right = GetHeightImpl(node->right);
        return 1 + std::max(left, right);
    }
};

}  // namespace simple

struct Rect {
    bool ext;
    Rect() : ext(false) {
    }
};

struct Line {
    int lef;
    int rig;
    int y_pos;
    bool open;
    std::shared_ptr<Rect> rect;
    bool point;

    Line(int left, int right, int yy, bool is_open, std::shared_ptr<Rect> rect_ptr,
         bool is_point = false)
        : lef(left), rig(right), y_pos(yy), open(is_open), rect(rect_ptr), point(is_point) {
    }

    bool IsExt() const {
        return rect->ext;
    }

    void SetExt() {
        rect->ext = true;
    }

    bool IsIn(Line* line) {
        return lef > line->lef;
    }

    bool IsPoint() const {
        return point;
    }

    void Print() const {
        std::cout << lef << " " << rig << " " << y_pos << std::endl;
    }
};

class Treap {
    struct Node {
        Line* key;
        int prior;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        explicit Node(Line* added_key)
            : key(added_key), prior(GenRandomInt()), left(nullptr), right(nullptr) {
        }
    };

    using NodePtr = std::shared_ptr<Node>;
    NodePtr root_;

public:
    Treap() : root_(nullptr) {
    }

    void Insert(Line* line) {
        auto splitted = Split(root_, line->rig);
        NodePtr new_node = std::make_shared<Node>(line);
        root_ = Merge(Merge(splitted.left, new_node), splitted.right);
    }

    void Remove(Line* line) {
        auto splitted = Split(root_, line->rig);
        root_ = Merge(splitted.left, splitted.right);
    }

    NodePtr Find(Line* line) {
        auto splitted = Split(root_, line->rig);
        root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
        return splitted.elem;
    }

    NodePtr UpperBound(int val) {
        auto splitted = Split(root_, val);
        auto elem = splitted.right;
        while (elem) {
            if (!elem->left || (elem->left && elem->left->key->rig < val)) {
                break;
            }
            elem = elem->left;
        }
        root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
        return elem;
    }

    void Print() const {
        PrintImpl(root_);
    }

private:
    struct Splitted {
        NodePtr left;
        NodePtr elem;
        NodePtr right;
    };

    NodePtr Merge(NodePtr left, NodePtr right) {
        if (!left || !right) {
            return left ? left : right;
        }
        if (left->prior > right->prior) {
            left->right = Merge(left->right, right);
            return left;
        }
        right->left = Merge(left, right->left);
        return right;
    }

    Splitted Split(NodePtr node, int val) {
        if (!node) {
            return {nullptr, nullptr, nullptr};
        }
        if (node->key->rig == val) {
            Splitted splitted{node->left, node, node->right};
            splitted.elem->left = nullptr;
            splitted.elem->right = nullptr;
            return splitted;
        }
        if (node->key->rig < val) {
            auto splitted = Split(node->right, val);
            node->right = splitted.left;
            return {node, splitted.elem, splitted.right};
        }
        auto splitted = Split(node->left, val);
        node->left = splitted.right;
        return {splitted.left, splitted.elem, node};
    }
    void PrintImpl(NodePtr node, std::string indent = "") const {
        if (!node) {
            return;
        }
        if (node == root_) {
            std::cout << "Treap\n";
        }
        std::cout << indent;
        node->key->Print();
        indent = indent.empty() ? "'-> " : "    " + indent;
        PrintImpl(node->left, indent);
        PrintImpl(node->right, indent);
    }
};

void ProcessData(int xx, int yy, int xxx, int yyy, std::vector<std::shared_ptr<Rect>>* rects,
                 std::vector<Line>* lines) {
    auto rect = std::make_shared<Rect>();
    rects->push_back(rect);

    if (xx == xxx && yy == yyy) {
        lines->emplace_back(xx, xxx, yy, true, rect, true);
        return;
    }
    int lef = xx < xxx ? xx : xxx;
    int rig = xx < xxx ? xxx : xx;
    int top = yy < yyy ? yyy : yy;
    int bot = yy < yyy ? yy : yyy;

    lines->emplace_back(lef, rig, bot, false, rect);
    lines->emplace_back(lef, rig, top, true, rect);
}

int GetExternalRect(std::vector<Line>& lines) {
    std::sort(lines.begin(), lines.end(), [](const Line& lhs, const Line& rhs) {
        if (lhs.y_pos == rhs.y_pos) {
            return lhs.rig < rhs.rig;
        }
        return lhs.y_pos > rhs.y_pos;
    });

    Treap treap;
    int ext = 0;

    for (int ind = 0; ind < static_cast<int>(lines.size()); ++ind) {
        auto& line = lines[ind];
        if (line.open) {
            auto node = treap.UpperBound(line.rig);
            if (!node) {
                if (!line.IsPoint()) {
                    line.SetExt();
                    treap.Insert(&lines[ind]);
                }
                ++ext;
            } else {
                if (!line.IsIn(node->key)) {
                    if (!line.IsPoint()) {
                        line.SetExt();
                        treap.Insert(&lines[ind]);
                    }
                    ++ext;
                }
            }
        } else if (line.IsExt()) {
            treap.Remove(&lines[ind]);
        }
    }
    return ext;
}

int GetExternalRect(const std::vector<std::vector<int>>& data) {
    std::vector<std::shared_ptr<Rect>> rects;
    std::vector<Line> lines;

    for (const auto& coords : data) {
        ProcessData(coords[0], coords[1], coords[2], coords[3], &rects, &lines);
    }

    return GetExternalRect(lines);
}

#ifndef LOCAL
int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    int count;
    std::cin >> count;

    std::vector<std::shared_ptr<Rect>> rects;
    std::vector<Line> lines;

    while (count > 0) {
        int xx, yy, xxx, yyy;
        std::cin >> xx >> yy >> xxx >> yyy;
        ProcessData(xx, yy, xxx, yyy, &rects, &lines);
        --count;
    }

    std::cout << GetExternalRect(lines) << '\n';
    return 0;
}

#endif
