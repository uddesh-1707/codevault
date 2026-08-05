// Date: 2026-08-01
// Problem: Predict the Winner
// Link: https://leetcode.com/problems/predict-the-winner/
// Code:

class Solution {
public: 
int solve(vector<int>& a, int l, int r) {
        if (l == r)
            return a[l];

        int left = a[l] - solve(a, l + 1, r);
        int right = a[r] - solve(a, l, r - 1);

        return max(left, right);
    }
    bool predictTheWinner(vector<int>& a) {
        // game theory task 
        int n = a.size();
        return solve(a, 0, n-1) >= 0;

        
    }
};