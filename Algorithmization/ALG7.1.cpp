#include <iostream>

using namespace std;

struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) {
        value = x;
        left = nullptr;
        right = nullptr;
    }
};

TreeNode* readTree() {
    int x;
    cin >> x;

    if (x == -1) return nullptr;

    TreeNode* root = new TreeNode(x);
    root->left = readTree();
    root->right = readTree();

    return root;
}

bool areTwins(TreeNode* first, TreeNode* second) {
    if (first == nullptr && second == nullptr) return true;
    if (first == nullptr || second == nullptr) return false;
    if (first->value != second->value) return false;

    return areTwins(first->left, second->left) &&
        areTwins(first->right, second->right);
}

void deleteTree(TreeNode* root) {
    if (root == nullptr) return;

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    TreeNode* first = readTree();
    TreeNode* second = readTree();

    if (areTwins(first, second)) {
        cout << "True" << endl;
    }
    else {
        cout << "False" << endl;
    }

    deleteTree(first);
    deleteTree(second);

    return 0;
}