// Date: 2026-08-04
// Problem: Coin Change
// Link: https://leetcode.com/problems/coin-change/
// Code:

class Solution {
public:
    int coinChange(vector<int>& a, int amount) {
        // for reaching particular amount find minimum number of coin
        int n = a.size();
        const int INF = amount+1;
        vector<int> dp(amount+1,INF); // think again here;
        dp[0] = 0;
        for(int i = 1 ; i <= amount ; i++){
            for(int j = 0 ; j < n ; j++){
                if(i - a[j] >= 0){
                    dp[i] = min(dp[i],dp[i-a[j]] + 1);
                }
            }
        }
        return (dp[amount] == INF) ? -1 : dp[amount];
    }
};