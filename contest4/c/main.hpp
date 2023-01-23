#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <stack>
#include <utility>
#include <string>

struct TreeNode {
    int key;
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;

    explicit TreeNode(int val) : key(val), left(nullptr), right(nullptr) {
    }
    TreeNode(int val, std::shared_ptr<TreeNode> left_n, std::shared_ptr<TreeNode> right_n)
        : key(val), left(left_n), right(right_n) {
    }
};

class FastList {
    std::vector<std::shared_ptr<TreeNode>> data_;

public:
    explicit FastList(int size) : data_() {
        data_.reserve(size);
    }

    void Add(std::shared_ptr<TreeNode> node) {
        data_.push_back(node);
    }

    std::shared_ptr<TreeNode> Erase(int val) {
        std::shared_ptr<TreeNode> res = nullptr;
        if (data_.empty()) {
            return res;
        }

        auto it = std::lower_bound(
            data_.rbegin(), data_.rend(), val,
            [](const std::shared_ptr<TreeNode>& node, int value) { return node->key < value; });
        if (it == data_.rend()) {
            res = data_.front();
            data_.clear();
            return res;
        }
        if (it == data_.rbegin()) {
            res = data_.back();
            data_.pop_back();
            return res;
        }
        if ((*it)->key != val) {
            --it;
        }
        res = *it;
        data_.erase((it + 1).base(), data_.end());
        return res;
    }
};

class Bst {
    using Sequence = std::vector<int>;
    std::shared_ptr<TreeNode> root_;
    int size_;

public:
    Bst() : root_(nullptr), size_(0) {
    }
    explicit Bst(std::shared_ptr<TreeNode> root) : root_(root) {
    }
    void RecoverFromPreorder(const Sequence& data) {
        if (data.empty()) {
            return;
        }
        size_ = data.size();
        FastList list(data.size());
        auto node = std::make_shared<TreeNode>(data[0]);
        list.Add(node);
        root_ = node;

        int pos = 1;
        while (pos < static_cast<int>(data.size())) {
            if (data[pos] < data[pos - 1]) {
                node->left = std::make_shared<TreeNode>(data[pos]);
                node = node->left;
                list.Add(node);
                ++pos;
                continue;
            }
            auto it = list.Erase(data[pos]);
            it->right = std::make_shared<TreeNode>(data[pos]);
            node = it->right;
            list.Add(node);
            ++pos;
        }
    }
    Sequence Preorder() {
        Sequence data;
        data.reserve(size_);
        if (root_ == nullptr) {
            return data;
        }
        std::stack<std::shared_ptr<TreeNode>> node_stack;
        node_stack.push(root_);
        while (!node_stack.empty()) {
            auto temp_node = node_stack.top();
            if (!temp_node) {
                node_stack.pop();
                continue;
            }

            data.push_back(temp_node->key);
            node_stack.pop();
            node_stack.push(temp_node->right);
            node_stack.push(temp_node->left);
        }
        return data;
    }
    std::pair<Sequence, Sequence> InPostOrder() {
        Sequence inorder;
        inorder.reserve(size_);
        Sequence postorder;
        postorder.reserve(size_);
        InPostOrderRec(root_, &inorder, &postorder);
        return {postorder, inorder};
    }

    void Print() {
        PrintRec(root_);
    }

private:
    void InPostOrderRec(std::shared_ptr<TreeNode> node, Sequence* inorder, Sequence* postorder) {
        if (!node) {
            return;
        }
        InPostOrderRec(node->left, inorder, postorder);
        inorder->push_back(node->key);
        InPostOrderRec(node->right, inorder, postorder);
        postorder->push_back(node->key);
    }

    void PrintRec(std::shared_ptr<TreeNode> node, std::string indent = "") {
        if (!node) {
            return;
        }
        std::cout << indent << node->key << '\n';
        indent = node == root_ ? "'-> " : "    " + indent;
        PrintRec(node->left, indent);
        PrintRec(node->right, indent);
    }
};

#ifndef LOCAL
int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    int v_count;
    std::cin >> v_count;

    std::vector<int> postorder;
    postorder.reserve(v_count);

    Bst tree;

    while (v_count > 0) {
        int val;
        std::cin >> val;
        postorder.push_back(val);
        --v_count;
    }

    tree.RecoverFromPreorder(postorder);
    auto pair = tree.InPostOrder();

    for (auto val : pair.first) {
        std::cout << val << ' ';
    }
    std::cout << '\n';

    for (auto val : pair.second) {
        std::cout << val << ' ';
    }
    std::cout << '\n';

    return 0;
}

#endif
