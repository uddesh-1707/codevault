// Date: 2026-08-01
// Problem: Count Valid Prefixes
// Link: https://leetcode.com/problems/count-valid-prefixes/
// Code:

class Solution {
public:
    int countValidPrefixes(string s) {
        int n = s.length() , z = 0 , o = 0 ;
        int cnt = 0;
        for(int i = 0 ; i < n ; i++){
            if(s[i] == '0') o++;
            else z++;
            if(z == o-1 || o == z-1 || o == z) cnt++;
        }
        return cnt;
        
    }
};