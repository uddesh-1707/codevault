// Date: 2026-08-08
// Problem: Valid Palindrome II
// Link: https://leetcode.com/problems/valid-palindrome-ii/
// Code:

class Solution {
public:
    bool check(string &s, int lo, int hi) {
        while (lo < hi) {
            if (s[lo] != s[hi])
                return false;
            lo++;
            hi--;
        }
        return true;
    }

    bool validPalindrome(string s) {
        int lo = 0, hi = s.length() - 1;

        while (lo < hi) {
            if (s[lo] != s[hi]) {
                return check(s, lo + 1, hi) || check(s, lo, hi - 1);
            }
            lo++;
            hi--;
        }
        return true;
    }
};