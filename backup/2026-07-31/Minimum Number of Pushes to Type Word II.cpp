// Date: 2026-07-31
// Problem: Minimum Number of Pushes to Type Word II
// Link: https://leetcode.com/problems/minimum-number-of-pushes-to-type-word-ii/
// Code:

class Solution {
public:
    int minimumPushes(string s) {
        // same soln as easy version does
        // as i have handled for duplicate and till 1e5
        int n = s.length();
        vector<int> f(26, 0);
        for (int i = 0; i < n; i++) {
            f[s[i] - 'a']++;
        }
        sort(f.rbegin(), f.rend());
        int cnt = 0;
        for (int i = 0; i < 26; i++) {
            if (i >= 0 && i <= 7) {
                cnt += f[i] * 1;
            } else if (i >= 8 && i <= 15) {
                cnt += f[i] * 2;
            } else if (i >= 16 && i <= 23) {
                cnt += f[i] * 3;
            } else {
                cnt += f[i] * 4;
            }
        }
        return cnt;
    }
};