// Date: 2026-09-29
// Problem:  Check if There Is a Valid Parentheses String Path
// Link: https://leetcode.com/problems/check-if-there-is-a-valid-parentheses-string-path/
// Code:

class Solution {
public:
    int n, m;
    int dp[101][101][201];
    bool hasValidPath(vector<vector<char>>& a) {
        // there must be dp soln to this with path reconstruction
        // bottom up is tough to come up with
        n = a.size();
        m = a[0].size();
        if (a[0][0] == ')' || a[n - 1][m - 1] == '(')
            return false;
        if ((n + m - 1) % 2 == 1)
            return false;
        memset(dp, -1, sizeof(dp));

        for (int x = n - 1; x >= 0; x--) {
            for (int y = m - 1; y >= 0; y--) {
                for (int cnt = 0; cnt <= x + y + 1; cnt++) {
                    if (x == n - 1 && y == m - 1) {
                        dp[n - 1][m - 1][cnt] = (cnt == 0);
                        continue;
                    }
                    dp[x][y][cnt] = false;
                    if (x + 1 < n) {
                        int new_cnt = (a[x + 1][y] == '(') ? cnt + 1 : cnt - 1;
                        if (new_cnt >= 0 && dp[x + 1][y][new_cnt] == true) {
                            dp[x][y][cnt] = true;
                        }
                    }
                    if (y + 1 < m) {
                        int new_cnt = (a[x][y + 1] == '(') ? cnt + 1 : cnt - 1;
                        if (new_cnt >= 0 && dp[x][y + 1][new_cnt] == true) {
                            dp[x][y][cnt] = true;
                        }
                    }
                }
            }
        }
        return dp[0][0][1];
    }
};