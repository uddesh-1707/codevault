// Date: 2026-08-15
// Problem: Minimum Operations to Make a Rotated Palindrome I
// Link: https://leetcode.com/problems/minimum-operations-to-make-a-rotated-palindrome-i/
// Code:

class Solution {
public:
    int minOperations(string s) {
        int n = s.length();
        int ans = INT_MAX;
        for (int i = 0; i < n; i++) {
            // i is ops
            int lo = 0, hi = n - 1, cnt = i;
            while (lo < hi) {
                char val1 = s[(lo+i)%n];
                char val2 = s[(hi+i)%n];
                if (val1 != val2) {
                    // check for ops
                    cnt +=
                        min((val1 - val2 + 26) % 26, (val2 - val1 + 26) % 26);
                }
               lo++;
               hi--;
            }
            ans = min(ans, cnt);
        }
        return ans;
    }
};