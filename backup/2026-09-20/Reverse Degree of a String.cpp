// Date: 2026-09-20
// Problem: Reverse Degree of a String
// Link: https://leetcode.com/problems/reverse-degree-of-a-string/
// Code:

class Solution {
public:
    int reverseDegree(string s) {
        int n = s.length() , ans = 0;
        for(int i = 0 ; i < n ; i++){
            int c = s[i]-'a';
            ans += (i+1)*(26-c);
        }
        return ans;
        
    }
};