// Date: 2026-07-29
// Problem: Palindrome Number
// Link: https://leetcode.com/problems/palindrome-number/
// Code:

class Solution {
public:
    bool isPalindrome(int x) {
        string s = to_string(x);
        int lo = 0 , hi = s.length()-1;
        while(lo <hi){
            if(s[lo] != s[hi]) return false;
            lo++;
            hi--;
        }
        return true;
        
    }
};