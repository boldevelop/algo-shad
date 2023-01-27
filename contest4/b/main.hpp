#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <random>
#include <numeric>
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

        Node(int added_key) : key(added_key), prior(GenRandomInt()), left(nullptr), right(nullptr) {
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

    Line(int left, int right, int yy, bool is_open, std::shared_ptr<Rect> rect_ptr)
        : lef(left), rig(right), y_pos(yy), open(is_open), rect(rect_ptr) {
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
};

class Treap {
    struct Node {
        Line* key;
        int prior;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(Line* added_key) : key(added_key), prior(GenRandomInt()), left(nullptr), right(nullptr) {
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
        root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
        return splitted.right;
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
};

void ProcessData(int xx, int yy, int xxx, int yyy, std::vector<std::shared_ptr<Rect>>* rects,
                 std::vector<Line>* lines) {
    int lef = xx < xxx ? xx : xxx;
    int rig = xx < xxx ? xxx : xx;
    int top = yy < yyy ? yyy : yy;
    int bot = yy < yyy ? yy : yyy;
    auto rect = std::make_shared<Rect>();
    rects->push_back(rect);
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

    for (int i = 0; i < static_cast<int>(lines.size()); ++i) {
        auto& line = lines[i];
        if (line.open) {
            auto node = treap.UpperBound(line.rig);
            if (!node) {
                line.SetExt();
                treap.Insert(&lines[i]);
                ++ext;
            }
        } else if (line.IsExt()) {
            treap.Remove(&lines[i]);
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

    std::vector<Rect> data;
    while (count > 0) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
    }

    // std::cout << GetExternalRect(data) << '\n';
    return 0;
}

#endif
