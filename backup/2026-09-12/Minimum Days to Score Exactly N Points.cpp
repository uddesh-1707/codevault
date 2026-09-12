// Date: 2026-09-12
// Problem: Minimum Days to Score Exactly N Points
// Link: https://leetcode.com/problems/minimum-days-to-score-exactly-n-points/
// Code:

class Solution {
public:
    int minDays(int n) {
        // for this dp should work
        const int INF = 1e9;
        vector<int> dp(n+1,INF);
        dp[0]=0;
        for(int i = 0 ; i <= n ; i++){
            if(dp[i] == INF) continue;
            for(int j = 1 ; ; j++){
                int val = j*(j+1)/2;
                if(i+val > n) break;
                int c = (i == 0) ? j : j+1;
                dp[i+val] = min(dp[i+val],dp[i]+c);
            }
        }
        return dp[n];
    }
};