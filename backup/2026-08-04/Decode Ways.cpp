// Date: 2026-08-04
// Problem: Decode Ways
// Link: https://leetcode.com/problems/decode-ways/
// Code:

class Solution {
public:
    int numDecodings(string s) {
        int n = s.length();
        vector<int> dp(n + 1, 0); //
        dp[0] = 1;

        for (int i = 1; i <= n; i++) {

            dp[i] = (s[i - 1] >= '1' && s[i - 1] <= '9') ? dp[i - 1] : 0;
            if (i - 2 >= 0) {
                string v = (s.substr(i - 2, 2));
                dp[i] += (v >= "10" && v <= "26") ? dp[i - 2] : 0;
            }
        }
        return dp[n];
    }
};