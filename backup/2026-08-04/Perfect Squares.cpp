// Date: 2026-08-04
// Problem: Perfect Squares
// Link: https://leetcode.com/problems/perfect-squares/
// Code:

class Solution {
public:
    int numSquares(int n) {
        vector<int> dp(n+1,INT_MAX);
        dp[0] = 0;
        for(int i = 1 ; i <= n ; i++){
            for(int j = 1 ; j*j <= i ; j++){
                dp[i] = min(dp[i],dp[i-j*j]+1);
            }
        }
        return dp[n];
    }
};