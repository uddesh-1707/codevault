// Date: 2026-08-14
// Problem: Maximum Length Substring With Two Occurrences
// Link: https://leetcode.com/problems/maximum-length-substring-with-two-occurrences/
// Code:

class Solution {
public:
    int maximumLengthSubstring(string s) {
        int n = s.length(), ans = 0,i=0;
        vector<int> f(26, 0);
        for (int j = 0; j < n; j++) {
            f[s[j] - 'a']++;
            while(f[s[j] - 'a'] > 2) {
                f[s[i]-'a']--;
                i++;
            }
            ans = max(ans, j - i + 1);
        }
        return ans;
    }
};