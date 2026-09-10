// Date: 2026-09-10
// Problem: Count Nodes Equal to Average of Subtree
// Link: https://leetcode.com/problems/count-nodes-equal-to-average-of-subtree/
// Code:

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left),
 * right(right) {}
 * };
 */
class Solution {
public:
    int ans = 0;
    pair<int, int> solve(TreeNode* root) {
        if (!root)
            return {0, 0};

        auto l = solve(root->left);
        auto r = solve(root->right);

        int sum = l.first + r.first + root->val;
        int cnt = l.second + r.second + 1;

        if (root->val == sum / cnt)
            ans++;

        return {sum, cnt};
    }

    int averageOfSubtree(TreeNode* root) {
        // go bottom up and check if current node
        // is avg of its subtree or not
        solve(root);
        return ans;
    }
};